//
// Created by GameRock on 11/01/2026.
//


#include "../Vectors/Vector2D.hpp"
#include "../Vectors/Vector3D.hpp"
#include "../Vectors/Vector4D.hpp"

#include <sstream>
#include <cereal/archives/json.hpp>
#include <catch2/catch_test_macros.hpp>

using vfMath::Vector2D;
using vfMath::Vector3D;
using vfMath::Vector4D;

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

    SECTION("Serialization test")
    {
        std::stringstream ss;

        {
            cereal::JSONOutputArchive oarchive(ss);
            oarchive(cereal::make_nvp("vector", v1));
        }

        Vector2D<double> loaded;
        {
            cereal::JSONInputArchive iarchive(ss);
            iarchive(cereal::make_nvp("vector", loaded));
        }

        REQUIRE(loaded.x == v1.x);
        REQUIRE(loaded.y == v1.y);
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

    SECTION("Serialization test")
    {
        std::stringstream ss;

        {
            cereal::JSONOutputArchive oarchive(ss);
            oarchive(cereal::make_nvp("vector", v1));
        }

        Vector3D<double> loaded;
        {
            cereal::JSONInputArchive iarchive(ss);
            iarchive(cereal::make_nvp("vector", loaded));
        }

        REQUIRE(loaded == v1);
    }
}

TEST_CASE("Vector4D Basic Operations", "[vector]")
{
    Vector4D v1(1.0, 2.0, 3.0, 4.0);
    Vector4D v2(3.0, 4.0, 5.0, 6.0);

    SECTION("Addition works correctly")
    {
        auto result = v1 + v2;
        REQUIRE(result.x == 4.0);
        REQUIRE(result.y == 6.0);
        REQUIRE(result.z == 8.0);
        REQUIRE(result.w == 10.0);
    }

    // SECTION("Magnitude calculation")
    // {
    //     REQUIRE(v2.length() == std::sqrt(86.0));
    // }

    SECTION("Dot product calculation")
    {
        auto dot_product = v1.dot(v2);
        REQUIRE(dot_product == 50.0);
    }

    SECTION("Multiplication check")
    {
        Vector4D result = v1 * 5.0;
        REQUIRE(result.x == 5.0);
        REQUIRE(result.y == 10.0);
        REQUIRE(result.z == 15.0);
        REQUIRE(result.w == 20.0);
    }

    SECTION("Serialization test")
    {
        std::stringstream ss;

        {
            cereal::JSONOutputArchive oarchive(ss);
            oarchive(cereal::make_nvp("vector", v1));
        }

        Vector4D<double> loaded;
        {
            cereal::JSONInputArchive iarchive(ss);
            iarchive(cereal::make_nvp("vector", loaded));
        }

        REQUIRE(loaded == v1);
    }
}
