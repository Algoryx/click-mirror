find_package(cppzmq REQUIRED)

set(PUBLIC_INCLUDE_SOURCES
    include/click/Client.h
    include/click/Server.h
    include/click/ControlMessage.h
    include/click/ControlMessageBuilder.h
    include/click/DllExport.h
    include/click/ErrorMessage.h
    include/click/ErrorMessageBuilder.h
    include/click/HandshakeMessage.h
    include/click/HandshakeMessageBuilder.h
    include/click/HandshakeInitMessage.h
    include/click/HandshakeInitMessageBuilder.h
    include/click/Message.h
    include/click/MessageSerializer.h
    include/click/ResetMessage.h
    include/click/ResetMessageBuilder.h
    include/click/SensorMessage.h
    include/click/SensorMessageBuilder.h
    include/click/SensorRequestMessage.h
    include/click/SensorRequestMessageBuilder.h
)

add_library(click)
target_sources(click
    PRIVATE
        src/ErrorMessage.cpp
        src/ErrorMessageBuilder.cpp
        src/Client.cpp
        src/Server.cpp
        src/ControlMessage.cpp
        src/ControlMessageBuilder.cpp
        src/HandshakeMessage.cpp
        src/HandshakeMessageBuilder.cpp
        src/HandshakeInitMessage.cpp
        src/HandshakeInitMessageBuilder.cpp
        src/Message.cpp
        src/MessageFactory.cpp
        src/MessageSerializer.cpp
        src/ResetMessage.cpp
        src/ResetMessageBuilder.cpp
        src/SensorRequestMessage.cpp
        src/SensorRequestMessageBuilder.cpp
        src/SensorMessage.cpp
        src/SensorMessageBuilder.cpp
        ${PUBLIC_INCLUDE_SOURCES}
        include/click/MessageFactory.h
 )
if (BUILD_SHARED_LIBS)
    target_compile_definitions(click PRIVATE BUILD_SHARED_LIBS)
endif()
target_compile_features(click PRIVATE cxx_std_17)
  
target_link_libraries(click
    PRIVATE
        cppzmq
        clickprotobuf
)
target_include_directories(click 
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  
        $<INSTALL_INTERFACE:include/click>
)

add_subdirectory(tests)

if(NOT BUILD_SHARED_LIBS)
    set(CLICKPROTOBUF clickprotobuf)
endif()

# From https://www.youtube.com/watch?v=bsXLMQ6WgIk 38:54
install(TARGETS 
        click
        ${CLICKPROTOBUF}
    EXPORT ClickTargets    
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(EXPORT ClickTargets
    FILE ClickTargets.cmake
    NAMESPACE Click::
    DESTINATION lib/cmake/click
)

install(FILES 
    ${PUBLIC_INCLUDE_SOURCES}
    DESTINATION include/click
)

# Version handling, From https://www.youtube.com/watch?v=bsXLMQ6WgIk 42:00
include(CMakePackageConfigHelpers)
write_basic_package_version_file("ClickConfigVersion.cmake"
    VERSION ${CLICK_VERSION}
    COMPATIBILITY SameMajorVersion
)
if(BUILD_SHARED_LIBS)
    set(CLICKCONFIGPATH "shared_conf/")
endif()
install(FILES "${CLICKCONFIGPATH}ClickConfig.cmake" "${CMAKE_BINARY_DIR}/click/ClickConfigVersion.cmake"
    DESTINATION lib/cmake/click
)
