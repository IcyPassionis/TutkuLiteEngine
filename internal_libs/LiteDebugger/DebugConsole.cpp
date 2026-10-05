#include "LiteDebugger.h"

#include <deque>
#include <iostream>
#include <mutex>
#include <streambuf>

static std::mutex debugLogMutex;
static std::deque<std::string> debugLogLines;

struct DebugOutputBuffer : std::streambuf
{
    std::streambuf* originalBuffer = nullptr;
    std::string pendingLine;

    void FinishLine()
    {
        if (debugLogLines.size() == maximumDebugLogLines)
        {
            debugLogLines.pop_front();
        }
        debugLogLines.push_back(pendingLine);
        pendingLine.clear();
    }

    void Capture(const char* text, const std::streamsize textLength)
    {
        for (std::streamsize characterIndex = 0; characterIndex < textLength; ++characterIndex)
        {
            const char character = text[characterIndex];
            if (character == '\n')
            {
                FinishLine();
                continue;
            }
            if (pendingLine.size() >= maximumDebugLogLineLength)
            {
                continue;
            }

            // Keep control characters from changing the console layout.
            if (static_cast<unsigned char>(character) < 32)
            {
                pendingLine += ' ';
            }
            else
            {
                pendingLine += character;
            }
        }
    }

    std::streamsize xsputn(const char* text, const std::streamsize textLength) override
    {
        const std::lock_guard<std::mutex> lock(debugLogMutex);
        const std::streamsize writtenLength = originalBuffer->sputn(text, textLength);
        Capture(text, writtenLength);
        return writtenLength;
    }

    int_type overflow(const int_type character) override
    {
        if (traits_type::eq_int_type(character, traits_type::eof()))
        {
            return traits_type::not_eof(character);
        }

        const char characterValue = traits_type::to_char_type(character);
        if (xsputn(&characterValue, 1) != 1)
        {
            return traits_type::eof();
        }
        return character;
    }

    int sync() override
    {
        const std::lock_guard<std::mutex> lock(debugLogMutex);
        return originalBuffer->pubsync();
    }
};

static DebugOutputBuffer debugStandardOutput;
static DebugOutputBuffer debugErrorOutput;
static unsigned int consoleCaptureCount = 0;

DebugConsoleCapture::DebugConsoleCapture()
{
    if (consoleCaptureCount++ != 0)
    {
        return;
    }
    debugStandardOutput.originalBuffer = std::cout.rdbuf(&debugStandardOutput);
    debugErrorOutput.originalBuffer = std::cerr.rdbuf(&debugErrorOutput);
}

DebugConsoleCapture::~DebugConsoleCapture()
{
    if (--consoleCaptureCount != 0)
    {
        return;
    }
    std::cout.flush();
    std::cerr.flush();
    std::cout.rdbuf(debugStandardOutput.originalBuffer);
    std::cerr.rdbuf(debugErrorOutput.originalBuffer);
}

std::vector<std::string> GetDebugLogLines()
{
    const std::lock_guard<std::mutex> lock(debugLogMutex);
    std::vector<std::string> logLines(debugLogLines.begin(), debugLogLines.end());
    if (!debugStandardOutput.pendingLine.empty())
    {
        logLines.push_back(debugStandardOutput.pendingLine);
    }
    if (!debugErrorOutput.pendingLine.empty())
    {
        logLines.push_back(debugErrorOutput.pendingLine);
    }
    if (logLines.size() > maximumDebugLogLines)
    {
        const auto excessLineCount = static_cast<std::ptrdiff_t>(logLines.size() - maximumDebugLogLines);
        logLines.erase(logLines.begin(), logLines.begin() + excessLineCount);
    }
    return logLines;
}

void ClearDebugLog()
{
    const std::lock_guard<std::mutex> lock(debugLogMutex);
    debugLogLines.clear();
    debugStandardOutput.pendingLine.clear();
    debugErrorOutput.pendingLine.clear();
}
