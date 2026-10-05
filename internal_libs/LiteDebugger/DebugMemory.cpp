#include "LiteDebugger.h"

#include <algorithm>
#include <fstream>
#include <limits>
#include <sstream>

#ifdef __linux__
#include <dlfcn.h>
#include <filesystem>
#include <unordered_set>
#include <unistd.h>

struct DebugNvmlDevice
{
    void* handle = nullptr;
};

struct DebugNvmlProcess
{
    unsigned int pid;
    unsigned long long usedGpuMemory;
    unsigned int gpuInstanceId;
    unsigned int computeInstanceId;
};

static constexpr int nvmlSuccess = 0;
static constexpr int nvmlInsufficientSize = 7;
static constexpr unsigned int maximumNvmlProcesses = 4096;
static constexpr unsigned int nvmlProcessBufferPadding = 16;

struct DebugNvmlReader
{
    void* library = nullptr;
    bool isInitialized = false;
    int (*initialize)() = nullptr;
    int (*shutdown)() = nullptr;
    int (*getDeviceCount)(unsigned int*) = nullptr;
    int (*getDevice)(unsigned int, DebugNvmlDevice*) = nullptr;
    int (*getProcesses)(DebugNvmlDevice, unsigned int*, DebugNvmlProcess*) = nullptr;

    DebugNvmlReader()
    {
        library = dlopen("libnvidia-ml.so.1", RTLD_LAZY | RTLD_LOCAL);
        if (!library)
        {
            return;
        }
        initialize = reinterpret_cast<decltype(initialize)>(dlsym(library, "nvmlInit_v2"));
        shutdown = reinterpret_cast<decltype(shutdown)>(dlsym(library, "nvmlShutdown"));
        getDeviceCount = reinterpret_cast<decltype(getDeviceCount)>(dlsym(library, "nvmlDeviceGetCount_v2"));
        getDevice = reinterpret_cast<decltype(getDevice)>(dlsym(library, "nvmlDeviceGetHandleByIndex_v2"));
        getProcesses = reinterpret_cast<decltype(getProcesses)>(dlsym(library, "nvmlDeviceGetGraphicsRunningProcesses_v3"));
        if (!getProcesses)
        {
            getProcesses = reinterpret_cast<decltype(getProcesses)>(dlsym(library, "nvmlDeviceGetGraphicsRunningProcesses_v2"));
        }
        const bool hasRequiredFunctions = initialize && shutdown && getDeviceCount && getDevice && getProcesses;
        if (hasRequiredFunctions)
        {
            isInitialized = initialize() == nvmlSuccess;
        }
    }

    ~DebugNvmlReader()
    {
        if (isInitialized)
        {
            shutdown();
        }
        if (library)
        {
            dlclose(library);
        }
    }

    bool ReadDeviceProcessMemory(const DebugNvmlDevice device, std::uint64_t& bytes) const
    {
        unsigned int processCount = 0;
        const int countResult = getProcesses(device, &processCount, nullptr);
        if (countResult != nvmlSuccess && countResult != nvmlInsufficientSize)
        {
            return false;
        }
        if (processCount == 0 || processCount > maximumNvmlProcesses)
        {
            return false;
        }

        // Leave room for processes created between the count and data queries.
        std::vector<DebugNvmlProcess> processes(processCount + nvmlProcessBufferPadding);
        processCount = static_cast<unsigned int>(processes.size());
        if (getProcesses(device, &processCount, processes.data()) != nvmlSuccess)
        {
            return false;
        }

        bool isAvailable = false;
        const unsigned int currentProcessId = static_cast<unsigned int>(getpid());
        const auto unavailableMemory = std::numeric_limits<unsigned long long>::max();
        for (unsigned int processIndex = 0; processIndex < processCount; ++processIndex)
        {
            const DebugNvmlProcess& process = processes[processIndex];
            if (process.pid != currentProcessId || process.usedGpuMemory == unavailableMemory)
            {
                continue;
            }
            bytes += process.usedGpuMemory;
            isAvailable = true;
        }
        return isAvailable;
    }

    bool Read(std::uint64_t& bytes) const
    {
        if (!isInitialized)
        {
            return false;
        }
        unsigned int deviceCount = 0;
        if (getDeviceCount(&deviceCount) != nvmlSuccess)
        {
            return false;
        }

        bool isAvailable = false;
        bytes = 0;
        for (unsigned int deviceIndex = 0; deviceIndex < deviceCount; ++deviceIndex)
        {
            DebugNvmlDevice device;
            if (getDevice(deviceIndex, &device) != nvmlSuccess)
            {
                continue;
            }
            if (ReadDeviceProcessMemory(device, bytes))
            {
                isAvailable = true;
            }
        }
        return isAvailable;
    }
};

static bool ReadMemorySize(std::istringstream& value, std::uint64_t& bytes)
{
    std::uint64_t amount = 0;
    std::string unit;
    if (!(value >> amount >> unit))
    {
        return false;
    }

    if (unit == "KiB")
    {
        amount *= 1024;
    }
    else if (unit == "MiB")
    {
        amount *= 1024 * 1024;
    }
    else if (unit != "B")
    {
        return false;
    }

    bytes = amount;
    return true;
}

struct DebugDrmClientMemory
{
    std::string clientId;
    std::string deviceId;
    std::uint64_t bytes = 0;
    bool isAvailable = false;
};

static DebugDrmClientMemory ReadDrmClientMemory(const std::filesystem::path& filePath)
{
    std::ifstream input(filePath);
    DebugDrmClientMemory clientMemory;
    std::string line;
    while (std::getline(input, line))
    {
        const std::size_t separator = line.find(':');
        if (separator == std::string::npos)
        {
            continue;
        }

        const std::string key = line.substr(0, separator);
        std::istringstream value(line.substr(separator + 1));
        if (key == "drm-client-id")
        {
            value >> clientMemory.clientId;
        }
        else if (key == "drm-pdev")
        {
            value >> clientMemory.deviceId;
        }
        else if (key.starts_with("drm-memory-vram"))
        {
            std::uint64_t allocationBytes = 0;
            if (ReadMemorySize(value, allocationBytes))
            {
                clientMemory.bytes += allocationBytes;
                clientMemory.isAvailable = true;
            }
        }
    }
    return clientMemory;
}

static bool ReadDrmVram(std::uint64_t& bytes)
{
    std::error_code error;
    const std::filesystem::directory_iterator files("/proc/self/fdinfo", error);
    if (error)
    {
        return false;
    }
    std::unordered_set<std::string> visitedClients;
    bool isAvailable = false;
    bytes = 0;
    for (auto iterator = files; iterator != std::filesystem::directory_iterator{}; iterator.increment(error))
    {
        if (error)
        {
            break;
        }
        const DebugDrmClientMemory clientMemory = ReadDrmClientMemory(iterator->path());
        if (!clientMemory.isAvailable || clientMemory.clientId.empty() || clientMemory.deviceId.empty())
        {
            continue;
        }

        // Duplicated descriptors share one DRM client: never count them twice.
        const std::string clientKey = clientMemory.deviceId + ":" + clientMemory.clientId;
        if (visitedClients.insert(clientKey).second)
        {
            bytes += clientMemory.bytes;
            isAvailable = true;
        }
    }
    return isAvailable;
}

static bool ReadProcessRam(std::uint64_t& bytes)
{
    std::ifstream status("/proc/self/status");
    std::string line;
    while (std::getline(status, line))
    {
        if (!line.starts_with("VmRSS:"))
        {
            continue;
        }

        std::istringstream value(line.substr(6));
        std::uint64_t kilobytes = 0;
        if (!(value >> kilobytes))
        {
            return false;
        }
        bytes = kilobytes * 1024;
        return true;
    }
    return false;
}
#endif

void DebugMemoryStatistics::Sample()
{
    isRamAvailable = false;
    isVramAvailable = false;
    ramBytes = 0;
    vramBytes = 0;
#ifdef __linux__
    isRamAvailable = ReadProcessRam(ramBytes);
    isVramAvailable = ReadDrmVram(vramBytes);
    if (!isVramAvailable)
    {
        static DebugNvmlReader nvml;
        isVramAvailable = nvml.Read(vramBytes);
    }
#endif
    if (isRamAvailable)
    {
        peakRamBytes = std::max(peakRamBytes, ramBytes);
    }
    if (isVramAvailable)
    {
        peakVramBytes = std::max(peakVramBytes, vramBytes);
    }
}

void DebugMemoryStatistics::ResetPeaks()
{
    peakRamBytes = ramBytes;
    peakVramBytes = vramBytes;
}
