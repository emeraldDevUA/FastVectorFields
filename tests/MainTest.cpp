#include <iostream>
#include <fstream>

#include "../VectorFields/VectorField2D.h"
#include "../ScalarFields/ScalarField2D.h"
#include "../FieldBase/AbstractField2D.h"
#include "../Vectors/Vector2D.h"

using namespace vfMath;

int main()
{
    // ScalarField2D<double> scalar_field(20, 20);
    // scalar_field.fill([](double x, double y) {
    //     const double r = std::sqrt(x * x + y * y);
    //     return std::sin(10.0 * r);
    // }, -1.0, 1.0, -1.0, 1.0);
    //
    // std::cout << scalar_field << std::endl;
    //
    // VectorField2D<double> vector_field(scalar_field);
    //
    // std::ofstream file("data.txt");
    //
    // file << vector_field;

    return 0;
}
