
add_executable(click-tests)
target_sources(click-tests
    PRIVATE
        test_srcdir.cpp
        test_control_message.cpp
        test_inithandshake_message.cpp
        test_protobuf_messages.cpp
        integrationtest_sensor_message.cpp
        integrationtest_handshake_message.cpp
        test_protobuf_message_integration.cpp
        test_message.cpp
        catch_main.cpp
    )
target_link_libraries(click-tests
    PRIVATE
        clicklib
        clickprotobuf # Only needed for test_protobuf_*.cpp
        CONAN_PKG::catch2
)

include(Catch)
catch_discover_tests(click-tests)

