add_library(clickprotobuf)
target_sources(clickprotobuf PRIVATE Messaging.proto)

find_package(Protobuf REQUIRED)
target_link_libraries(clickprotobuf
    PUBLIC
        protobuf::libprotobuf
    )
target_include_directories(clickprotobuf 
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>  
        $<INSTALL_INTERFACE:include/click>
    )

protobuf_generate(TARGET clickprotobuf LANGUAGE cpp)
# Update generated python
protobuf_generate(TARGET clickprotobuf LANGUAGE python)
add_custom_command(TARGET clickprotobuf POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/Messaging_pb2.py ${PROJECT_SOURCE_DIR}/../python-src/src/Messaging_pb2.py
)
