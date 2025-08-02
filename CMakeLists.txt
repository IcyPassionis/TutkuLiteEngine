cmake_minimum_required(VERSION 3.31)
#C++ Version 20
set(CMAKE_CXX_STANDARD 20)

project(Engine)

add_subdirectory(external_libs/raylib) #adds raylib subdirectory
add_subdirectory(internal_libs/Lighting) #adds our Lighting subdirectory
add_subdirectory(internal_libs/LiteDebugger) # Adds our debugger subdirectory

# defining MY_SOURCE to be list of all the source files for project
file(GLOB_RECURSE MY_SOURCE CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_executable(Engine "${MY_SOURCE}")

#Resources path for development purposes
target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
#Game file paths for development purposes
target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC GAME_PATH="${CMAKE_CURRENT_SOURCE_DIR}/")

#Resources path for build purposes, uncomment it and comment development path for building the game !
#target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="./resources/")

#Header files
target_include_directories(Engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(Engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/States")
target_include_directories(Engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/Managers")

#Linking libraries
add_library(raygui INTERFACE)
add_library(json INTERFACE)

target_include_directories(raygui INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/external_libs/raygui
        ${CMAKE_CURRENT_SOURCE_DIR}/external_libs/raylib
        ${CMAKE_CURRENT_SOURCE_DIR}/external_libs/json
)

target_link_libraries(Engine PRIVATE raygui) # Linking raygui to engine
target_link_libraries(Engine PRIVATE json)  # Linking json to engine
target_link_libraries(raygui INTERFACE raylib) # Linking raylib to raygui
target_link_libraries(Engine INTERFACE raygui) # Linking immediate gui of raylib, External
target_link_libraries(Engine PRIVATE lighting_library) # Linking lighting-shader library, internal
target_link_libraries(Engine PRIVATE lite_debugger) # Linking debugger library, internal
