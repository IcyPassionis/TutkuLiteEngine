# Minimum cmake version
cmake_minimum_required(VERSION 3.31)
#C++ standart 20
set(CMAKE_CXX_STANDARD 20)

#Include source files and makes it an library
add_library(lighting_library STATIC LightingManager.cpp)
#Includes headers
target_include_directories(lighting_library PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#Includes raylib
target_link_libraries(lighting_library PRIVATE raylib)
target_link_libraries(lighting_library PRIVATE raygui)

target_compile_definitions(lighting_library PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/shaders/")