# R3D v0.11 calls find_package(raylib), but Engine already created the only
# raylib target from its bundled raylib 5.5 source tree.
if(TARGET raylib)
    set(raylib_FOUND TRUE)
    set(RAYLIB_FOUND TRUE)
    set(RAYLIB_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/external_libs/raylib/src")
    set(RAYLIB_LIBRARIES raylib)
else()
    set(raylib_FOUND FALSE)
    set(RAYLIB_FOUND FALSE)
endif()
