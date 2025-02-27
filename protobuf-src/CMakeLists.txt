add_library(clickprotobuf STATIC)
target_sources(clickprotobuf PRIVATE Messaging.proto)
target_compile_features(clickprotobuf PRIVATE cxx_std_17)
# Avoid linking issue with click built as a shared library
if(BUILD_SHARED_LIBS)
  set_target_properties(clickprotobuf PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

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
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/Messaging_pb2.py ${PROJECT_SOURCE_DIR}/../python-src/src/pclick/Messaging_pb2.py
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/Messaging.pb.h ${PROJECT_SOURCE_DIR}/../cpp-src/click/include
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/Messaging.pb.cc ${PROJECT_SOURCE_DIR}/../cpp-src/click/src
)
