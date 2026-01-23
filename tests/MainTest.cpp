#include <iostream>
#include <fstream>

#include "../VectorFields/VectorField2D.h"
#include "../ScalarFields/ScalarField2D.h"
#include "../FieldBase/AbstractField2D.h"
#include "../Vectors/Vector2D.h"
#include  "../Vectors/Vector3D.h"

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


    VectorField2D<double> new_vector_field_2d(4, 4);

    new_vector_field_2d.setValue(1, 1, Vector2D(-1.0, 0.0));
    new_vector_field_2d.setValue(3, 3, Vector2D(0.0, 1.0));

    new_vector_field_2d.fillWithInterpolation();

    std::cout << new_vector_field_2d << std::endl;


    return 0;
}
