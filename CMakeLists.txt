cmake_minimum_required(VERSION 3.31)
#C++ Version 20
set(CMAKE_CXX_STANDARD 20)
project(Engine)
#adds raylib subdirectory
add_subdirectory(external_libs/raylib)
#adds our Lighting subdirectory
add_subdirectory(internal_libs/Lighting)
# defining MY_SOURCE to be list of all the source files for project
file(GLOB_RECURSE MY_SOURCE CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(Engine "${MY_SOURCE}")
#Resources path for development purposes
target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
#Resources path for build purposes, uncomment it and comment development path for building the game !
#target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="./resources/")

#Header files
target_include_directories(Engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(Engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/States")
target_include_directories(Engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/Managers")

#Linking libraries
target_link_libraries(Engine PRIVATE raylib) # Linking raylib, easier game-rendering library, external
target_link_libraries(Engine PRIVATE lighting_library)