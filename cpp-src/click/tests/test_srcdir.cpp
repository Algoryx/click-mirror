#include <catch2/catch_test_macros.hpp>
#include "testpaths.h"

SCENARIO("source dir", "[test helpers]") {
    REQUIRE(srcDir("dir/src.cpp") == "dir");
    REQUIRE(srcDir("dir\\src.cpp") == "dir");
}
