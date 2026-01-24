//
// Created by GameRock on 11/01/2026.
//
#include <catch2/catch_test_macros.hpp>

#include "../Vectors/Vector2D.h"
#include "../Vectors/Vector3D.h"

using vfMath::Vector2D;

using vfMath::Vector3D;

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
        REQUIRE(v2.length() == 5.0);
    }

    SECTION("Dot product calculation")
    {
        auto dot_product = v1.dot(v2);
        REQUIRE(dot_product == 11.0);
    }

    SECTION("Multiplication check")
    {
        Vector2D result = v1 * 5.0;
        REQUIRE(result.x == 5.0);
        REQUIRE(result.y == 10.0);
    }

}



TEST_CASE("Vector3D Basic Operations", "[vector]")
{
    Vector3D v1(1.0, 2.0, 3.0);
    Vector3D v2(3.0, 4.0, 5.0);

    SECTION("Addition works correctly")
    {
        auto result = v1 + v2;
        REQUIRE(result.x == 4.0);
        REQUIRE(result.y == 6.0);
        REQUIRE(result.z == 8.0);
    }

    // SECTION("Magnitude calculation")
    // {
    //     REQUIRE(v2.length() == 5.0);
    // }

    SECTION("Dot product calculation")
    {
        auto dot_product = v1.dot(v2);
        REQUIRE(dot_product == 26.0);
    }

    SECTION("Multiplication check")
    {
        Vector3D result = v1 * 5.0;
        REQUIRE(result.x == 5.0);
        REQUIRE(result.y == 10.0);
        REQUIRE(result.z == 15.0);
    }

}

