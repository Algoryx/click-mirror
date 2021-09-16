# From https://www.f-ax.de/dev/2020/11/08/grpc-plugin-cmake-support.html
#find_package(protobuf CONFIG REQUIRED)
# TODO: Why can't I use CONAN_PKG::protobuf as dependency instead? (It doesn't work - but why?)
find_package(Protobuf REQUIRED)
#find_package(Threads)

set(PROTO_FILES
    Messaging.proto
)

# Add Library target with protobuf sources
add_library(clicklib ${PROTO_FILES})
target_link_libraries(clicklib
    PUBLIC
        protobuf::libprotobuf
)
target_include_directories(clicklib PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

protobuf_generate(TARGET clicklib LANGUAGE cpp)
