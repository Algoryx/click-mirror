find_package(Protobuf REQUIRED)

set(LIBSOURCES
    include/click/MessageFactory.h
    src/MessageFactory.cpp
)

add_library(click_lib ${LIBSOURCES})
target_link_libraries(click_lib
    PUBLIC
        protobuf::libprotobuf
        clicklib
)
target_include_directories(click_lib 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SOURCES
    src/client.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

add_executable(client ${SOURCES})
target_link_libraries(client
    PRIVATE
    click_lib
    CONAN_PKG::zmqpp
)
