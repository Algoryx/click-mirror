#include <catch2/catch.hpp>
#include "TestPaths.h"

SCENARIO("source dir", "[test helpers]") {
    REQUIRE(srcDir("dir/src.cpp") == "dir");
    REQUIRE(srcDir("dir\\src.cpp") == "dir");
}
