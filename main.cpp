#include <iostream>
#include "VectorFields/VectorField2D.h"
#include "FieldBase/AbstractField2D.h"
#include "Vectors/Vector2D.h"

using namespace vfMath;
// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main()
{

    VectorField2D<Vector2D<double>> field(10, 10);

    field.setValue( 3, 4, Vector2D(1.0, 2.0) );

    std::cout << "Vector at (3,4): (" << field.getValue(3,1).x << ", " << field.getValue(3,1).y << ")\n";

    return 0;
}