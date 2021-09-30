# From https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md#cmake-targets
Include(FetchContent)

set(TEST_EXECUTABLE_TARGET click-test)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.0.0-preview3
)

FetchContent_MakeAvailable(Catch2)

set(TEST_SOURCES
    test_srcdir.cpp
    test_control_message.cpp
    test_inithandshake_message.cpp
    test_protobuf_messages.cpp
    integrationtest_sensor_message.cpp
    integrationtest_handshake_message.cpp
    test_protobuf_message_integration.cpp
)

add_executable(${TEST_EXECUTABLE_TARGET} ${TEST_SOURCES})
target_link_libraries(${TEST_EXECUTABLE_TARGET}
    PRIVATE
    clicklib
    Catch2::Catch2WithMain
)

# Test execution target
# From https://stackoverflow.com/questions/32901679/unit-testing-as-part-of-the-build
add_custom_target(test
  DEPENDS ${TEST_EXECUTABLE_TARGET}
)

add_custom_command(
     TARGET test
     COMMENT "Run tests"
     POST_BUILD
     COMMAND ${TEST_EXECUTABLE_TARGET}
)
