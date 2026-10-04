# R3D v0.11 calls find_package(assimp). Assimp is populated at the pinned
# upstream submodule revision before R3D is added as a subdirectory.
if(TARGET assimp)
    set(_ASSIMP_TARGET assimp)
elseif(TARGET assimp::assimp)
    set(_ASSIMP_TARGET assimp::assimp)
endif()

if(_ASSIMP_TARGET)
    set(assimp_FOUND TRUE)
    set(ASSIMP_FOUND TRUE)
    if(assimp_SOURCE_DIR)
        set(ASSIMP_INCLUDE_DIRS
            "${assimp_SOURCE_DIR}/include;${assimp_BINARY_DIR}/include")
    else()
        get_target_property(ASSIMP_INCLUDE_DIRS ${_ASSIMP_TARGET}
            INTERFACE_INCLUDE_DIRECTORIES)
    endif()
    set(ASSIMP_LIBRARIES ${_ASSIMP_TARGET})
else()
    set(assimp_FOUND FALSE)
    set(ASSIMP_FOUND FALSE)
endif()
