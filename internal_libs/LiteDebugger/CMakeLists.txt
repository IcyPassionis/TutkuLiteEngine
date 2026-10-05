# Minimum cmake version
cmake_minimum_required(VERSION 3.16)
#C++ standart 20
set(CMAKE_CXX_STANDARD 20)
#Include source files and makes it an library
file(GLOB_RECURSE DEBUGGER_SOURCE CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_library(lite_debugger STATIC ${DEBUGGER_SOURCE})
#Includes headers
target_include_directories(lite_debugger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(lite_debugger PRIVATE raygui ${CMAKE_DL_LIBS})
