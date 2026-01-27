#include <iostream>
#include <fstream>

#include "../VectorFields/VectorField2D.h"
#include "../ScalarFields/ScalarField2D.h"
#include "../FieldBase/AbstractField2D.h"
#include "../Vectors/Vector2D.h"
#include  "../Vectors/Vector3D.h"

#include  "../Interpolation/RBFInterpolator2D.h"

#include <cereal/types/memory.hpp>

using namespace vfMath;

int main()
{
    ScalarField2D<double> scalar_field(20, 20);

    scalar_field.fill([](double x, double y)
    {
        return std::sin(8 * M_PI * x) * std::sin(8 * M_PI * y);
    }, -1.0, 1.0, -1.0, 1.0);

    std::cout << scalar_field << std::endl;

    VectorField2D<double> vector_field(scalar_field);
    vector_field.normalize();
    std::ofstream file("data.txt");

    //file << vector_field;

    for (int i = 0; i < 20; ++i)
    {
        for (int j = 0; j < 20; ++j)
        {
            double div = vector_field.curl(i, j);
            std::cout << div << " ";
        }
        std::cout << std::endl;
    }


    size_t vector_field_size = 20;
    VectorField2D<double> new_vector_field_2d(vector_field_size, vector_field_size);

    new_vector_field_2d.setValue(10, 0, {0.0, 1.0});
    new_vector_field_2d.setValue(10, 19, {0.0, -1.0});
    new_vector_field_2d.setValue(0, 10, {1.0, 0.0});
    new_vector_field_2d.setValue(19, 10, {-1.0, 0.0});


    new_vector_field_2d.fillWithInterpolation();
    new_vector_field_2d.normalize();
    //file << new_vector_field_2d;

    VectorField2D<double> a(16, 16);

    a.setValue(0, 0, {1.0, 0.0});
    a.setValue(3, 3, {-1.0, 0});
    a.setValue(1, 1, {0.5, -0.5});

    a.fillWithInterpolation();

    file << a;

    std::ofstream os("out.json", std::ios::binary);
    cereal::JSONOutputArchive archive(os);

    archive(cereal::make_nvp<>("vector_field", a));

    return 0;
}
