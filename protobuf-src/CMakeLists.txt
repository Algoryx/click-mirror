# From https://www.f-ax.de/dev/2020/11/08/grpc-plugin-cmake-support.html
# TODO: Discuss difference btw CONFIG and without - CONFIG is recommended?
#find_package(protobuf CONFIG REQUIRED)
find_package(Protobuf REQUIRED)

set(PROTO_FILES
    Messaging.proto
)

# Add Library target with protobuf sources
add_library(clickprotobuf ${PROTO_FILES})
target_link_libraries(clickprotobuf
    PUBLIC
        CONAN_PKG::protobuf
)
target_include_directories(clickprotobuf PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

protobuf_generate(TARGET clickprotobuf LANGUAGE cpp)
# Update generated python
protobuf_generate(TARGET clickprotobuf LANGUAGE python)
add_custom_command(TARGET clickprotobuf POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/Messaging_pb2.py ${PROJECT_SOURCE_DIR}/../python-src/src/Messaging_pb2.py
)
