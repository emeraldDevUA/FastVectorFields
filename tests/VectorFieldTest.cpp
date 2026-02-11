//
// Created by GameRock on 23/01/2026.
//

//
// Created by GameRock on 11/01/2026.
//


#include "../Vectors/Vector2D.hpp"
#include "../Vectors/Vector3D.hpp"

#include "../VectorFields/VectorField2D.hpp"
#include "../VectorFields/VectorField3D.hpp"

#include <catch2/catch_test_macros.hpp>

using vfMath::Vector2D;
using vfMath::Vector3D;

using vfFields::VectorField2D;
using vfFields::VectorField3D;


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

    SECTION("Addition works correctly")
    {
        VectorField2D<double> a(3, 3);
        VectorField2D<double> b(3, 3);

        a.setValue(0, 2, {1.0, 2.0});
        b.setValue(0, 2, {3.0, -2.0});

        auto result = a - b;

        REQUIRE(result.getValue(0, 2).x == -2.0);
        REQUIRE(result.getValue(0, 2).y == 4.0);
    }


    SECTION("Interpolated sampling works correctly")
    {
        VectorField2D<double> a(3, 3);

        a.setValue(0, 0, {1.0, 1.0});
        a.setValue(1, 1, {-1.0, -1.0});

        auto result = a(0.5, 0.5);
        REQUIRE(result == Vector2D<double>(0.0, 0.0));
    }

    SECTION("Comparison test")
    {
        VectorField2D<double> a(3, 3);

        a.setValue(0, 0, {1.0, 1.0});
        a.setValue(1, 1, {-1.0, -1.0});

        VectorField2D<double> b(3, 3);

        b.setValue(0, 0, {1.0, 1.0});
        b.setValue(1, 1, {-1.0, -1.0});

        REQUIRE(a != vector_field_2d_2);
        REQUIRE(a == b);
    }


    SECTION("Interpolated sampling works correctly")
    {
        VectorField2D<double> a(16, 16);

        a.setValue(0, 0, {1.0, 0.0});
        a.setValue(3, 3, {-1.0, 0});
        a.setValue(1, 1, {0.5, -0.5});

        a.fillWithInterpolation();


        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                auto v = a.getValue(i, j);
                REQUIRE(v.x <= 1.0);
                REQUIRE(v.x >= -1.0);
                REQUIRE(v.y <= 1.0);
                REQUIRE(v.y >= -1.0);
            }
        }
    }

    SECTION("Serialization test")
    {
        std::stringstream ss;

        {
            cereal::JSONOutputArchive oarchive(ss);
            oarchive(cereal::make_nvp("vector_field", vector_field_2d_1));
        }

        VectorField2D<double> loaded(3, 3);
        {
            cereal::JSONInputArchive iarchive(ss);
            iarchive(cereal::make_nvp("vector_field", loaded));
        }

        REQUIRE(loaded == vector_field_2d_1);
    }
}

TEST_CASE("VectorField3D Operations", "[vector]")
{
    VectorField3D<double> vector_field_3d_1(3, 3, 3);
    VectorField3D<double> vector_field_3d_2(3, 3, 3);

    SECTION("Values are set correctly")
    {
        vector_field_3d_1.setValue(0, 2, 1, {1.0, 2.0, 3.0});

        REQUIRE(vector_field_3d_1.getValue(0, 2, 1).x == 1.0);
        REQUIRE(vector_field_3d_1.getValue(0, 2, 1).y == 2.0);
        REQUIRE(vector_field_3d_1.getValue(0, 2, 1).z == 3.0);

        vector_field_3d_2.setValue(0, 2, 1, {3.0, -2.0, 1.0});

        REQUIRE(vector_field_3d_2.getValue(0, 2, 1).x == 3.0);
        REQUIRE(vector_field_3d_2.getValue(0, 2, 1).y == -2.0);
        REQUIRE(vector_field_3d_2.getValue(0, 2, 1).z == 1.0);
    }

    SECTION("Addition works correctly")
    {
        VectorField3D<double> a(3, 3, 3);
        VectorField3D<double> b(3, 3, 3);

        a.setValue(0, 2, 1, {1.0, 2.0, 3.0});
        b.setValue(0, 2, 1, {3.0, -2.0, 1.0});

        auto result = a + b;

        REQUIRE(result.getValue(0, 2, 1).x == 4.0);
        REQUIRE(result.getValue(0, 2, 1).y == 0.0);
        REQUIRE(result.getValue(0, 2, 1).z == 4.0);
    }

    SECTION("Subtraction works correctly")
    {
        VectorField3D<double> a(3, 3, 3);
        VectorField3D<double> b(3, 3, 3);

        a.setValue(0, 2, 1, {1.0, 2.0, 3.0});
        b.setValue(0, 2, 1, {3.0, -2.0, 1.0});

        auto result = a - b;

        REQUIRE(result.getValue(0, 2, 1).x == -2.0);
        REQUIRE(result.getValue(0, 2, 1).y == 4.0);
        REQUIRE(result.getValue(0, 2, 1).z == 2.0);
    }

    SECTION("Interpolated sampling works correctly")
    {
        VectorField3D<double> a(3, 3, 3);

        a.setValue(0, 0, 0, {1.0, 1.0, 1.0});
        a.setValue(1, 1, 1, {-1.0, -1.0, -1.0});

        auto result = a(0.5, 0.5, 0.5);
        REQUIRE(result == Vector3D<double>(0.0, 0.0, 0.0));
    }

    SECTION("Comparison test")
    {
        VectorField3D<double> a(3, 3, 3);
        VectorField3D<double> b(3, 3, 3);

        a.setValue(0, 0, 0, {1.0, 1.0, 1.0});
        a.setValue(1, 1, 1, {-1.0, -1.0, -1.0});

        b.setValue(0, 0, 0, {1.0, 1.0, 1.0});
        b.setValue(1, 1, 1, {-1.0, -1.0, -1.0});

        REQUIRE(a != vector_field_3d_2);
        REQUIRE(a == b);
    }

    SECTION("Interpolated fill works correctly")
    {
        VectorField3D<double> a(16, 16, 16);

        a.setValue(0, 0, 0, {1.0, 0.0, 0.0});
        a.setValue(3, 3, 3, {-1.0, 0.0, 0.0});
        a.setValue(1, 1, 1, {0.5, -0.5, 0.5});

        a.fillWithInterpolation();

        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                for (int k = 0; k < 4; ++k)
                {
                    auto v = a.getValue(i, j, k);
                    REQUIRE(v.x <= 1.0);
                    REQUIRE(v.x >= -1.0);
                    REQUIRE(v.y <= 1.0);
                    REQUIRE(v.y >= -1.0);
                    REQUIRE(v.z <= 1.0);
                    REQUIRE(v.z >= -1.0);
                }
            }
        }
    }

    SECTION("Serialization test")
    {
        std::stringstream ss;

        {
            cereal::JSONOutputArchive oarchive(ss);
            oarchive(cereal::make_nvp("vector_field", vector_field_3d_1));
        }

        VectorField3D<double> loaded(3, 3, 3);
        {
            cereal::JSONInputArchive iarchive(ss);
            iarchive(cereal::make_nvp("vector_field", loaded));
        }

        REQUIRE(loaded == vector_field_3d_1);
    }
}
