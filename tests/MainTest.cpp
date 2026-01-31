#include <iostream>
#include <fstream>
#include <cereal/types/memory.hpp>

#include "../VectorFields/VectorField2D.h"
#include "../ScalarFields/ScalarField2D.h"
#include "../FieldBase/AbstractField2D.h"
#include "../Vectors/Vector2D.h"
#include  "../Vectors/Vector3D.h"

using vfMath::Vector2D;

using vfFields::VectorField2D;
using vfFields::ScalarField2D;

int main()
{
    ScalarField2D<double> scalar_field(32, 32);

    scalar_field.fill([](const double x, const double y)
    {
        const double r = std::sqrt(x * x + y * y);
        const double theta = std::atan2(y, x);
        return std::sin(8 * M_PI * r + 4 * theta);
    }, -1.0, 1.0, -1.0, 1.0);

    VectorField2D vector_field(scalar_field);
    vector_field.normalize();

    std::ofstream os1("scalar_field.json", std::ios::binary);
    cereal::JSONOutputArchive archive1(os1);

    archive1(cereal::make_nvp<>("scalar_field", scalar_field));

    std::ofstream os("vector_field.json", std::ios::binary);
    cereal::JSONOutputArchive archive(os);
    archive(cereal::make_nvp<>("vector_field", vector_field));


    return 0;
}
