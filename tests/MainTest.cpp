#include "../Vectors/Vector2D.hpp"
#include "../Vectors/Vector3D.hpp"

#include "../ScalarFields/ScalarField2D.hpp"
#include "../VectorFields/VectorField2D.hpp"

#include "../ScalarFields/ScalarField3D.hpp"
#include "../VectorFields/VectorField3D.hpp"

#include <iostream>
#include <fstream>
#include <cereal/types/memory.hpp>

using vfMath::Vector2D;

using vfFields::VectorField2D;
using vfFields::ScalarField2D;

using vfFields::VectorField3D;
using vfFields::ScalarField3D;


template <typename T>
void serializeToJson(const T& object, const std::string& filename, const std::string& name)
{
    std::ofstream os(filename);
    if (!os)
    {
        std::cerr << "Failed to open " << filename << " for writing.\n";
        return;
    }

    cereal::JSONOutputArchive archive(os);
    archive(cereal::make_nvp(name, object));
}

int main()
{
    ScalarField2D<double> scalar_field(128, 128);

    scalar_field.fill([](const double x, const double y)
    {
        const double r = std::sqrt(x * x + y * y);
        const double theta = std::atan2(y, x);
        return std::sin(8 * std::numbers::pi * r + 4 * theta);
    }, -1.0, 1.0, -1.0, 1.0);

    VectorField2D vector_field(scalar_field);
    vector_field.normalize();

    // Serialize using the new function
    serializeToJson(scalar_field, "scalar_field.json", "scalar_field");
    serializeToJson(vector_field, "vector_field.json", "vector_field");

    ScalarField3D<double> scalar_field3D(32, 32, 32);

    scalar_field3D.fill([](const double x, const double y, const double z)
    {
        const double r = std::sqrt(x * x + y * y);
        const double theta = std::atan2(y, x);
        return std::sin(8 * std::numbers::pi * r + 4 * theta);
    }, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    VectorField3D vector_field3D(scalar_field3D);
    vector_field.normalize();

    // Serialize using the new function
    serializeToJson(scalar_field, "scalar_field3D.json", "scalar_field");
    serializeToJson(vector_field, "vector_field3D.json", "vector_field");

    return 0;
}
