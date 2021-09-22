# From https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md#cmake-targets
Include(FetchContent)

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

add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests 
    PRIVATE
    clicklib
    Catch2::Catch2WithMain
)

set(UNIT_TEST tests)
# From https://stackoverflow.com/questions/32901679/unit-testing-as-part-of-the-build
# TODO: Could not get integration with ctest to work though
add_custom_command(
     TARGET ${UNIT_TEST}
     COMMENT "Run tests"
     POST_BUILD 
     COMMAND ${UNIT_TEST}
)
# junit reporter https://github.com/catchorg/Catch2/blob/devel/docs/reporters.md
add_custom_command(
     TARGET ${UNIT_TEST}
     COMMENT "Run tests"
     POST_BUILD 
     COMMAND ${UNIT_TEST} "--reporter junit" "--out junit.xml"
)
