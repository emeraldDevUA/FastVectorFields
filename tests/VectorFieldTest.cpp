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

        VectorField2D<double> result = a + b;

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

        auto result = a.sample(0.5, 0.5);
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

    SECTION("Constant field")
    {
        VectorField2D<double> field(3, 3);

        // F = (1, 1)
        for (size_t i = 0; i < 3; ++i)
            for (size_t j = 0; j < 3; ++j)
                field.setValue(i, j, {1, 1});

        // Divergence should be 0 everywhere
        for (size_t i = 1; i < 2; ++i)
            for (size_t j = 1; j < 2; ++j)
                REQUIRE(field.divergence(i, j) == (0.0));

        // Curl should be 0 everywhere
        for (size_t i = 1; i < 2; ++i)
            for (size_t j = 1; j < 2; ++j)
                REQUIRE(field.curl(i, j) == (0.0));
    }

    SECTION("Linear field F = (x, y)")
    {
        VectorField2D<double> field(5, 5);

        for (size_t i = 0; i < 5; ++i)
            for (size_t j = 0; j < 5; ++j)
                field.setValue(i, j, {static_cast<double>(i), static_cast<double>(j)});

        // Divergence = du/dx + dv/dy = 1 + 1 = 2
        for (size_t i = 1; i < 4; ++i)
            for (size_t j = 1; j < 4; ++j)
                REQUIRE(field.divergence(i, j) == (2.0));

        // Curl = dv/dx - du/dy = 0 - 0 = 0
        for (size_t i = 1; i < 4; ++i)
            for (size_t j = 1; j < 4; ++j)
                REQUIRE(field.curl(i, j) == (0.0));
    }

    SECTION("Rotational field F = (-y, x)")
    {
        VectorField2D<double> field(5, 5);

        for (size_t i = 0; i < 5; ++i)
            for (size_t j = 0; j < 5; ++j)
                field.setValue(i, j, {-static_cast<double>(j), static_cast<double>(i)});

        // Divergence = du/dx + dv/dy = 0
        for (size_t i = 1; i < 4; ++i)
            for (size_t j = 1; j < 4; ++j)
                REQUIRE(field.divergence(i, j) == (0.0));

        // Curl = dv/dx - du/dy = 1 - (-1) = 2
        for (size_t i = 1; i < 4; ++i)
            for (size_t j = 1; j < 4; ++j)
                REQUIRE(field.curl(i, j) == (2.0));
    }

    SECTION("Edge handling test")
    {
        VectorField2D<double> field(3, 3);

        for (size_t i = 0; i < 3; ++i)
            for (size_t j = 0; j < 3; ++j)
                field.setValue(i, j, {static_cast<double>(i), static_cast<double>(j)});

        // Test boundary (i=0, j=0) and (i=2, j=2) for safe behavior
        REQUIRE_NOTHROW(field.divergence(0, 0));
        REQUIRE_NOTHROW(field.curl(0, 0));
        REQUIRE_NOTHROW(field.divergence(2, 2));
        REQUIRE_NOTHROW(field.curl(2, 2));
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

        auto result = a.sample(0.5, 0.5, 0.5);
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

    SECTION("Constant field")
    {
        VectorField3D<double> field(3, 3, 3);

        // F = (1,1,1)
        for (size_t i = 0; i < 3; ++i)
            for (size_t j = 0; j < 3; ++j)
                for (size_t k = 0; k < 3; ++k)
                    field.setValue(i, j, k, {1, 1, 1});

        for (size_t i = 1; i < 2; ++i)
            for (size_t j = 1; j < 2; ++j)
                for (size_t k = 1; k < 2; ++k)
                {
                    REQUIRE(field.divergence(i, j, k) == (0.0));
                    Vector3D<double> c = field.curl(i, j, k);
                    REQUIRE(c.x == (0.0));
                    REQUIRE(c.y == (0.0));
                    REQUIRE(c.z == (0.0));
                }
    }

    SECTION("Linear field F = (x, y, z)")
    {
        VectorField3D<double> field(5, 5, 5);

        for (size_t i = 0; i < 5; ++i)
            for (size_t j = 0; j < 5; ++j)
                for (size_t k = 0; k < 5; ++k)
                    field.setValue(i, j, k, {
                                       static_cast<double>(i),
                                       static_cast<double>(j),
                                       static_cast<double>(k)
                                   });

        // Divergence = du/dx + dv/dy + dw/dz = 1+1+1 = 3
        for (size_t i = 1; i < 4; ++i)
            for (size_t j = 1; j < 4; ++j)
                for (size_t k = 1; k < 4; ++k)
                {
                    REQUIRE(field.divergence(i, j, k) == (3.0));

                    Vector3D<double> c = field.curl(i, j, k);
                    REQUIRE(c.x == (0.0));
                    REQUIRE(c.y == (0.0));
                    REQUIRE(c.z == (0.0));
                }
    }

    SECTION("Rotational field F = (-y, x, 0)")
    {
        VectorField3D<double> field(5, 5, 5);

        for (size_t i = 0; i < 5; ++i)
            for (size_t j = 0; j < 5; ++j)
                for (size_t k = 0; k < 5; ++k)
                    field.setValue(i, j, k, {
                                       -static_cast<double>(j),
                                       static_cast<double>(i),
                                       0
                                   });

        // Divergence = 0
        for (size_t i = 1; i < 4; ++i)
            for (size_t j = 1; j < 4; ++j)
                for (size_t k = 1; k < 4; ++k)
                    REQUIRE(field.divergence(i, j, k) == (0.0));

        // Curl = (0,0,2)
        for (size_t i = 1; i < 4; ++i)
            for (size_t j = 1; j < 4; ++j)
                for (size_t k = 1; k < 4; ++k)
                {
                    Vector3D<double> c = field.curl(i, j, k);
                    REQUIRE(c.x == (0.0));
                    REQUIRE(c.y == (0.0));
                    REQUIRE(c.z == (2.0));
                }
    }

    SECTION("Edge safety")
    {
        VectorField3D<double> field(3, 3, 3);

        for (size_t i = 0; i < 3; ++i)
            for (size_t j = 0; j < 3; ++j)
                for (size_t k = 0; k < 3; ++k)
                    field.setValue(i, j, k, {
                                       static_cast<double>(i),
                                       static_cast<double>(j),
                                       static_cast<double>(k)
                                   });

        REQUIRE_NOTHROW(field.divergence(0, 0, 0));
        REQUIRE_NOTHROW(field.divergence(2, 2, 2));

        REQUIRE_NOTHROW(field.curl(0, 0, 0));
        REQUIRE_NOTHROW(field.curl(2, 2, 2));
    }
}
