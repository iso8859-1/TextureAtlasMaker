
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "generator.hpp"

TEST_CASE("generated texture has the desired filename","[functional],[generator]")
{
    generateTexture("testfile.png");
    REQUIRE(false);
}