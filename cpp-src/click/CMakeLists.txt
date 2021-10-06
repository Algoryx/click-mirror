find_package(zmqpp REQUIRED)

set(PUBLIC_INCLUDE_SOURCES
    include/click/Client.h
    include/click/ControlMessage.h
    include/click/ControlMessageBuilder.h
    include/click/DllExport.h
    include/click/HandshakeInitMessage.h
    include/click/HandshakeInitMessageBuilder.h
    include/click/HandshakeMessage.h
    include/click/Message.h
    include/click/MessageSerializer.h
    include/click/SensorMessage.h
    )

#add_library(clicklib SHARED)
add_library(clicklib STATIC)
target_sources(clicklib
    PRIVATE
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
        ${PUBLIC_INCLUDE_SOURCES}
        include/click/MessageFactory.h
    )
target_link_libraries(clicklib
    PRIVATE
        zmqpp::zmqpp
    PUBLIC
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
install(FILES "ClickConfig.cmake" "${CMAKE_BINARY_DIR}/click/ClickConfigVersion.cmake"
    DESTINATION lib/cmake/click
    )
