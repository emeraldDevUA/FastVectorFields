//
// Created by GameRock on 23/01/2026.
//

//
// Created by GameRock on 11/01/2026.
//
#include <iostream>
#include <catch2/catch_test_macros.hpp>

#include "../Vectors/Vector2D.h"
#include "../Vectors/Vector3D.h"

#include "../VectorFields/VectorField2D.h"

using vfMath::Vector2D;
using vfMath::Vector3D;


TEST_CASE("VectorField Operations", "[vector]")
{
    VectorField2D<double> vector_field_2d_1(3, 3);
    VectorField2D<double> vector_field_2d_2(3, 3);

    SECTION("Values are set correctly correctly")
    {
        vector_field_2d_1.setValue(0, 2, {1.0, 2.0});


        REQUIRE(vector_field_2d_1.getValue(0, 2).x == 1.0);
        REQUIRE(vector_field_2d_1.getValue(0, 2).y == 2.0);

        vector_field_2d_2.setValue(0, 2, {3.0, -2.0});


        REQUIRE(vector_field_2d_2.getValue(0, 2).x == 3.0);
        REQUIRE(vector_field_2d_2.getValue(0, 2).y == -2.0);
    }

    SECTION("Addition works correctly")
    {
        VectorField2D<double> a(3, 3);
        VectorField2D<double> b(3, 3);

        a.setValue(0, 2, {1.0, 2.0});
        b.setValue(0, 2, {3.0, -2.0});

        auto result = a + b;

        REQUIRE(result.getValue(0, 2).x == 4.0);
        REQUIRE(result.getValue(0, 2).y == 0.0);
    }

}
