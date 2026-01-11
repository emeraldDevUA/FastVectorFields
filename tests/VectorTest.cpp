//
// Created by GameRock on 11/01/2026.
//
#include <catch2/catch_test_macros.hpp>

#include "../Vectors/Vector2D.h"

using vfMath::Vector2D;

TEST_CASE("Vector2D Basic Operations", "[vector]")
{
    Vector2D v1(1.0, 2.0);
    Vector2D v2(3.0, 4.0);

    SECTION("Addition works correctly")
    {
        auto result = v1 + v2;
        REQUIRE(result.x == 4.0);
        REQUIRE(result.y == 6.0);
    }

    SECTION("Magnitude calculation")
    {
        const Vector2D v(3.0, 4.0);
        REQUIRE(v.length() == 5.0);
    }
}
