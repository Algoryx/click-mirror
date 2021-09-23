find_package(Protobuf REQUIRED)

set(LIBSOURCES
    include/click/MessageFactory.h
    include/click/Message.h
    include/click/ControlMessage.h
    include/click/ControlMessageBuilder.h
    include/click/SensorMessage.h
    include/click/HandshakeMessage.h
    include/click/HandshakeInitMessage.h
    include/click/HandshakeInitMessageBuilder.h
    include/click/Client.h
    src/MessageFactory.cpp
    src/MessageSerializer.cpp
    src/Client.cpp
    src/Message.cpp
    src/ControlMessage.cpp
    src/ControlMessageBuilder.cpp
    src/HandshakeMessage.cpp
    src/HandshakeInitMessage.cpp
    src/HandshakeInitMessageBuilder.cpp
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

install(TARGETS 
    clicklib
    clickprotobuf
    DESTINATION lib
)
install(FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/click/Message.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/click/MessageFactory.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/click/Message.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/click/ControlMessage.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/click/ControlMessageBuilder.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/click/SensorMessage.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/click/HandshakeMessage.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/click/HandshakeInitMessage.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/click/HandshakeInitMessageBuilder.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/click/Client.h
    DESTINATION include/click
)
