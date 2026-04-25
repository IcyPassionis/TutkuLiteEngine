# Minimum cmake version
cmake_minimum_required(VERSION 3.16)
#C++ standart 20
set(CMAKE_CXX_STANDARD 20)

#Include source files and makes it an library
add_library(lighting_library STATIC src/LightingManager.cpp src/Lighting.cpp src/ShaderManager.cpp)
#Includes headers
target_include_directories(lighting_library PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
#Includes raylib
target_link_libraries(lighting_library PRIVATE raylib)
target_link_libraries(lighting_library PRIVATE raygui)

target_compile_definitions(lighting_library PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/shaders/")