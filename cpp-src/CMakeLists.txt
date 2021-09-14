# From https://cliutils.gitlab.io/modern-cmake/chapters/basics.html

cmake_minimum_required(VERSION 3.7...3.21)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(click VERSION 1.0
    DESCRIPTION "Messaging Library and tooling for effective communication between a controller and AGX using Brick"
    LANGUAGES CXX)


# Output dirs
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")
#message("modpath: ${CMAKE_MODULE_PATH}")

# Conan - deps
include(conanconf)

# Project directives
add_subdirectory(../protobuf-src ./protobuf-bin)
# TODO: Probably should rename to client later
add_subdirectory(src)
add_subdirectory(tests)
