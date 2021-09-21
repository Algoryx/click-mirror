find_package(Protobuf REQUIRED)

set(LIBSOURCES
    include/click/MessageFactory.h
    include/click/ControlMessage.h
    include/click/SensorMessage.h
    include/click/Client.h
    src/MessageFactory.cpp
    src/MessageSerializer.cpp
    src/Client.cpp
    src/Message.cpp
    src/ControlMessage.cpp
    src/SensorMessage.cpp
)

add_library(clicklib ${LIBSOURCES})
target_link_libraries(clicklib
    PUBLIC
        protobuf::libprotobuf
        CONAN_PKG::zmqpp
        clickprotobuf
)
target_include_directories(clicklib 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(tests)
