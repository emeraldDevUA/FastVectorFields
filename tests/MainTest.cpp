
#include "../Vectors/Vector2D.hpp"
#include "../Vectors/Vector3D.hpp"

#include "../FieldBase/AbstractField2D.hpp"
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


void serialize(const auto& scalar_field, const std::string& file_name, const std::string& type)
{
    std::ofstream os1(file_name + ".json", std::ios::binary);
    cereal::JSONOutputArchive archive1(os1);

    archive1(cereal::make_nvp<>(type, scalar_field));
}

int main()
{
    ScalarField2D<double> scalar_field(64, 64);

    scalar_field.fill([](const double x, const double y)
    {
        const double r = std::sqrt(x * x + y * y);
        const double theta = std::atan2(y, x);
        return std::sin(8 * M_PI * r + 4 * theta);
    }, -1.0, 1.0, -1.0, 1.0);

    VectorField2D vector_field(scalar_field);
    vector_field.normalize();

    serialize(scalar_field, "scalar_field", "scalar_field");

    serialize(vector_field, "vector_field", "vector_field");

    VectorField2D<double> interpolation_target(32, 32);

    interpolation_target.setValue(0, 0, {1.0, 0.0});
    interpolation_target.setValue(31, 31, {-1.0, 0});
    interpolation_target.setValue(20, 17, {0.5, -0.5});
    interpolation_target.setValue(13, 12, {-0.5, 0.5});

    interpolation_target.normalize();

    serialize(interpolation_target, "vector_field_poles", "vector_field");

    try
    {
        interpolation_target.fillWithInterpolation();
    }   catch (const std::logic_error& e) {

        std::cerr << "exception caught: " << e.what() << '\n';
    }

    serialize(interpolation_target, "vector_field_interpolation", "vector_field");

    std::cout << interpolation_target << std::endl;


    ScalarField3D<double> scalar_field_3d(64);

    scalar_field_3d.fill([](const double x, const double y, const double z) {
        return std::exp(-(x*x + y*y + z*z));
    }, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    VectorField3D<double> vector_field_3d(12);
    //vector_field_3d.normalize();

    // serialize(scalar_field_3d, "scalar_field_3d", "scalar_field");
    //
    // serialize(vector_field_3d, "vector_field_3d", "vector_field");

    vector_field_3d.setValue(0,0,0, {1,1,1});
    vector_field_3d.setValue(5,5,5, {-0.4,0.5,0.5});
    vector_field_3d.setValue(11,11,11, {-1,-1,-1});

    vector_field_3d.fillWithInterpolation();

    serialize(vector_field_3d, "vector_field_3d", "vector_field");

    return 0;
}
