#find_package(Protobuf REQUIRED)

set(SOURCES
    src/democlient.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

add_executable(democlient ${SOURCES})
target_link_libraries(democlient
    PRIVATE
    clicklib
    CONAN_PKG::zmqpp
)
