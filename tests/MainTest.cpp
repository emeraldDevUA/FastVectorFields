#include "../Vectors/Vector2D.hpp"
#include "../Vectors/Vector3D.hpp"

#include "../ScalarFields/ScalarField2D.hpp"
#include "../VectorFields/VectorField2D.hpp"

#include "../ScalarFields/ScalarField3D.hpp"
#include "../VectorFields/VectorField3D.hpp"

#include <iostream>
#include <fstream>
#include <cereal/types/memory.hpp>
#include <chrono>

using vfMath::Vector2D;

using vfFields::VectorField2D;
using vfFields::ScalarField2D;

using vfFields::VectorField3D;
using vfFields::ScalarField3D;

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;

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
    ScalarField2D<double> scalar_field(64, 64);

    scalar_field.fill([](const double x, const double y)
    {
        const double r = std::sqrt(x * x + y * y);
        const double theta = std::atan2(y, x);
        return std::sin(8 * M_PI * r + 4 * theta);
    }, -1.0, 1.0, -1.0, 1.0);

    VectorField2D vector_field(scalar_field);
    vector_field.normalize();

    serializeToJson(scalar_field, "scalar_field", "scalar_field");

    serializeToJson(vector_field, "vector_field", "vector_field");

    int N = 128;
    VectorField2D<double> interpolation_target(N, N);


    interpolation_target.setValue(0, 0, {1.0, 0.0});
    interpolation_target.setValue(N-1, N-1, {-1.0, 0});
    interpolation_target.setValue(20 * N / 32, 17 * N / 32, {0.5, -0.5});
    interpolation_target.setValue(13 * N / 32, 12 * N / 32, {-0.5, 0.5});

    interpolation_target.normalize();

    serializeToJson(interpolation_target, "vector_field_poles", "vector_field");

    try
    {
        interpolation_target.fillWithInterpolation();
        interpolation_target.normalize();

    }
    catch (const std::logic_error& e)
    {
        std::cerr << "exception caught: " << e.what() << '\n';
    }

    serializeToJson(interpolation_target, "vector_field_interpolation", "vector_field");

    //std::cout << interpolation_target << std::endl;


    ScalarField3D<double> scalar_field_3d(32, 32, 32);

    scalar_field_3d.fill([](const double x, const double y, const double z)
    {
        const double r = std::sqrt(x * x + y * y + z * z);
        const double theta = std::atan2(y, x);
        return std::sin(8 * M_PI * r + 4 * theta);
    }, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);


    N = 32;
    VectorField3D<double> vector_field_3d(N);
    //vector_field_3d.normalize();

    // serialize(scalar_field_3d, "scalar_field_3d", "scalar_field");
    //
    // serialize(vector_field_3d, "vector_field_3d", "vector_field");

    vector_field_3d.setValue(0, 0, 0, {1.0, 0.0, 0});
    vector_field_3d.setValue(N-1, N-1, N-1, {-1.0, 0, 0});
    vector_field_3d.setValue(20 * N / 32, 17 * N / 32, 20 * N / 32, {0.5, -0.5, -0.5});
    vector_field_3d.setValue(13 * N / 32, 20 * N / 32, 12, {-0.5, 0.5, 0.5});
    //

    serializeToJson(vector_field_3d, "vector_field_3d_poles", "vector_field");
    vector_field_3d.normalize();
    vector_field_3d.fillWithInterpolation();
    vector_field_3d.normalize();

    serializeToJson(vector_field_3d, "vector_field_3d_interpolation", "vector_field");



    return 0;
}
