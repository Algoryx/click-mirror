cmake_minimum_required(VERSION 3.15)
project(PackageTest CXX)

find_package(click CONFIG REQUIRED)

add_executable(example src/example.cpp)
target_link_libraries(example click::click)

set_target_properties(example PROPERTIES
        CXX_STANDARD 14
)
