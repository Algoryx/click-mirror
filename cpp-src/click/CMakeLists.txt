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
        CONAN_PKG::protobuf
        CONAN_PKG::zmqpp
        clickprotobuf
)
target_include_directories(clicklib 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  
        $<INSTALL_INTERFACE:include/click>
    )

add_subdirectory(tests)

# From https://www.youtube.com/watch?v=bsXLMQ6WgIk 38:54
install(TARGETS 
        clicklib
        clickprotobuf
    EXPORT ClickTargets    
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
    )
install(EXPORT ClickTargets
    FILE ClickTargets.cmake
    NAMESPACE Click::
    DESTINATION lib/cmake/Click
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
