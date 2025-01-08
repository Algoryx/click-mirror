# From https://cliutils.gitlab.io/modern-cmake/chapters/basics.html

cmake_minimum_required(VERSION 3.7...3.21)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(click VERSION 0.5.0
    DESCRIPTION "Messaging Library and tooling for effective communication between a controller and AGX Dynamics using Brick"
    LANGUAGES CXX)

set(CMAKE_DEBUG_POSTFIX "-d")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")
#message("modpath: ${CMAKE_MODULE_PATH}")

include(BuildType)
include(conanconf)
include(configure_cpack)
include(CTest)

# Project directives
add_subdirectory(../protobuf-src ./protobuf-gen)
add_subdirectory(click)
add_subdirectory(democlient)

