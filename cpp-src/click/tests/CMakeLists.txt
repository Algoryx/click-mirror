# TODO: Why can't I use conan?
#find_package(Catch2 2 REQUIRED)
# From https://github.com/catchorg/Catch2/blob/v2.x/docs/cmake-integration.md#installing-catch2-from-git-repository
Include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v2.13.7)
  #     GIT_TAG        v3.0.0-preview3

FetchContent_MakeAvailable(Catch2)


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
        catch_main.cpp
    )
target_link_libraries(click-tests
    PRIVATE
        clicklib
        Catch2::Catch2
)

include(Catch)
catch_discover_tests(click-tests)
