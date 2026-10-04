cmake_minimum_required(VERSION 3.22)

project(Engine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# raylib is the single raylib target used by both Engine and R3D.
add_subdirectory(external_libs/raylib)

include(FetchContent)

set(R3D_RAYLIB_VENDORED OFF CACHE BOOL "Use Engine's existing raylib target" FORCE)
set(R3D_ASSIMP_VENDORED OFF CACHE BOOL "Use the pinned Assimp fallback target" FORCE)
set(R3D_SUPPORT_ASSIMP ON CACHE BOOL "Enable model and animation importing" FORCE)
set(R3D_BUILD_EXAMPLES OFF CACHE BOOL "Build R3D examples" FORCE)
set(R3D_BUILD_DOCS OFF CACHE BOOL "Build R3D documentation" FORCE)

# R3D v0.11.0, exact release commit. Keep dependencies in the build tree.
FetchContent_Declare(
    r3d
    GIT_REPOSITORY https://github.com/Bigfoot71/r3d.git
    GIT_TAG 86303391c92e32418181a8133848a15ba02f060d
    GIT_SUBMODULES ""
    GIT_SUBMODULES_RECURSE FALSE
    GIT_PROGRESS TRUE
)

# Prefer an installed Assimp 6.0.2+ package. Otherwise use the exact Assimp
# 6.0.3 source revision recorded as a submodule by the pinned R3D commit.
find_package(assimp 6.0.2 QUIET CONFIG)
if(NOT assimp_FOUND)
    FetchContent_Declare(
        assimp
        GIT_REPOSITORY https://github.com/assimp/assimp.git
        GIT_TAG cf7b652190827878a4726709f0998ce170e55ce8
        GIT_PROGRESS TRUE
    )

    set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE)
    set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)
    set(ASSIMP_IGNORE_GIT_HASH ON CACHE BOOL "" FORCE)
    set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)
    set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
    set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
    set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
    set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE)
    set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
    set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
    foreach(importer COLLADA GLTF OBJ FBX IQM)
        set(ASSIMP_BUILD_${importer}_IMPORTER ON CACHE BOOL "" FORCE)
    endforeach()
    foreach(importer AMF BVH OFF COB STL 3DS AC ASE ASSBIN B3D DXF CSM HMP
            IRRMESH IRR LWO LWS M3D MD2 MD3 MD5 MDC MDL NFF NDO OGRE
            OPENGEX PLY MS3D BLEND IFC XGL Q3D Q3BSP RAW SIB SMD
            TERRAGEN 3D X X3D 3MF MMD)
        set(ASSIMP_BUILD_${importer}_IMPORTER OFF CACHE BOOL "" FORCE)
    endforeach()

    FetchContent_MakeAvailable(assimp)
endif()

# R3D uses find_package() rather than checking for an already-created target.
list(PREPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
FetchContent_MakeAvailable(r3d)

add_subdirectory(internal_libs/LiteDebugger)

file(GLOB_RECURSE MY_SOURCE CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_executable(Engine ${MY_SOURCE})

target_compile_definitions(Engine PUBLIC
    RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/"
    GAME_PATH="${CMAKE_CURRENT_SOURCE_DIR}/"
)

target_include_directories(Engine PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/States"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Managers"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Scene System"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Object System"
    "${r3d_SOURCE_DIR}/include"
    "${r3d_BINARY_DIR}/generated/include"
)

add_library(raygui INTERFACE)
add_library(json INTERFACE)

target_include_directories(raygui INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/external_libs/raygui"
    "${CMAKE_CURRENT_SOURCE_DIR}/external_libs/raylib"
    "${CMAKE_CURRENT_SOURCE_DIR}/external_libs/json"
)

target_link_libraries(raygui INTERFACE raylib)
target_link_libraries(Engine PRIVATE raygui json r3d lite_debugger)
