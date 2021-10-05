#include <catch2/catch.hpp>
#include "testpaths.h"

SCENARIO("source dir", "[test helpers]") {
    REQUIRE(srcDir("dir/src.cpp") == "dir");
    REQUIRE(srcDir("dir\\src.cpp") == "dir");
}
