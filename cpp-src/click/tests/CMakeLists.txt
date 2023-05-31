
add_executable(click-tests)
target_sources(click-tests
    PRIVATE
        CatchMain.cpp
        TestControlMessage.cpp
        TestErrorMessage.cpp
        TestHandshakeMessage.cpp
        TestInithandshakeMessage.cpp
        TestMessage.cpp
        TestProtobufMessages.cpp
        TestResetMessage.cpp
        TestSensorRequestMessage.cpp
        TestSensorMessage.cpp
        TestSrcDir.cpp
        IntegrationtestSensorMessage.cpp
        IntegrationtestHandshakeMessage.cpp
        IntegrationTestProtobufMessage.cpp
    )
target_compile_features(click-tests PRIVATE cxx_std_17)
target_link_libraries(click-tests
    PRIVATE
        click
        clickprotobuf # Only needed for test_protobuf_*.cpp
        CONAN_PKG::catch2
)

include(Catch)
catch_discover_tests(click-tests)

