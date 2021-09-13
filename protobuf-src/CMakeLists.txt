# From https://www.f-ax.de/dev/2020/11/08/grpc-plugin-cmake-support.html
find_package(protobuf CONFIG REQUIRED)
#find_package(Threads)

set(PROTO_FILES
    Messaging.proto
)

# Add Library target with protobuf sources
add_library(myproto ${PROTO_FILES})
target_link_libraries(myproto
    PUBLIC
        protobuf::libprotobuf
        gRPC::grpc
        gRPC::grpc++
)
target_include_directories(myproto PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

protobuf_generate(TARGET myproto LANGUAGE cpp)
