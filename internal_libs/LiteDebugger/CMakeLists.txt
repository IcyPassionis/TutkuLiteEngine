# Minimum cmake version
cmake_minimum_required(VERSION 3.16)
#C++ standart 20
set(CMAKE_CXX_STANDARD 20)
#Include source files and makes it an library
add_library(lite_debugger STATIC DebugManager.cpp)
#Includes headers
target_include_directories(lite_debugger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(lite_debugger PRIVATE raygui)
