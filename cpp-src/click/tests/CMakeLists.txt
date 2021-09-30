
# From https://github.com/catchorg/Catch2/blob/v2.x/docs/cmake-integration.md#installing-catch2-from-git-repository
Include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v2.13.7)

FetchContent_MakeAvailable(Catch2)

# From https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md#cmake-targets
# Include(FetchContent)
# FetchContent_Declare(
    #     Catch2
    #     GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    #     GIT_TAG        v3.0.0-preview3
    # )
    
    # FetchContent_MakeAvailable(Catch2)

    
set(TEST_EXECUTABLE_TARGET click-tests)
add_executable(${TEST_EXECUTABLE_TARGET})
target_sources(${TEST_EXECUTABLE_TARGET}
    PRIVATE
        test_srcdir.cpp
        test_control_message.cpp
        test_inithandshake_message.cpp
        test_protobuf_messages.cpp
        integrationtest_sensor_message.cpp
        integrationtest_handshake_message.cpp
        test_protobuf_message_integration.cpp
        catch_main.cpp
    )
target_link_libraries(${TEST_EXECUTABLE_TARGET}
    PRIVATE
        clicklib
        Catch2::Catch2
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
