#include <iostream>
#include "VectorFields/VectorField2D.h"
#include "ScalarFields/ScalarField2D.h"
#include "FieldBase/AbstractField2D.h"
#include "Vectors/Vector2D.h"

using namespace vfMath;
// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main()
{

    ScalarField2D<double> scalar_field(10, 10);
    scalar_field.fill([](double x, double y) {
        return std::sin(x * 2 * 3.14159) * std::cos(y * 2 * 3.14159);
    }, true);

    std::cout << scalar_field << std::endl;

    VectorField2D<double> vector_field(scalar_field);

    std::cout << vector_field << std::endl;

    return 0;
}