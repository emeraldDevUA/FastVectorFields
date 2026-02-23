#include "../Vectors/Vector2D.hpp"
#include "../Vectors/Vector3D.hpp"

#include "../ScalarFields/ScalarField2D.hpp"
#include "../VectorFields/VectorField2D.hpp"

#include "../ScalarFields/ScalarField3D.hpp"
#include "../VectorFields/VectorField3D.hpp"

#include <iostream>
#include <fstream>
#include <numbers>
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
    // ScalarField2D<double> scalar_field(128, 128);
    //
    // scalar_field.fill([](const double x, const double y)
    // {
    //     const double r = std::sqrt(x * x + y * y);
    //     const double theta = std::atan2(y, x);
    //     return std::sin(8 * std::numbers::pi * r + 4 * theta);
    // }, -1.0, 1.0, -1.0, 1.0);
    //
    // VectorField2D vector_field(scalar_field);
    // vector_field.normalize();
    //
    // // Serialize using the new function
    // serializeToJson(scalar_field, "scalar_field.json", "scalar_field");
    // serializeToJson(vector_field, "vector_field.json", "vector_field");
    //
    // ScalarField3D<double> scalar_field3D(32, 32, 32);
    //
    // scalar_field3D.fill([](const double x, const double y, const double z)
    // {
    //     const double r = std::sqrt(x * x + y * y + z*z);
    //     const double theta = std::atan2(y, x);
    //     return std::sin(8 * std::numbers::pi * r + 4 * theta);
    // }, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    //
    // const VectorField3D vector_field3D(scalar_field3D);
    // vector_field.normalize();
    //
    // // Serialize using the new function
    // serializeToJson(vector_field3D, "scalar_field3D.json", "scalar_field");
    // serializeToJson(vector_field3D, "vector_field3D.json", "vector_field");

    std::ofstream file("addition_benchmark.csv");
    file << "size,total_elements,time_ms\n";

    // Sizes to test (you can modify this)
    std::vector<size_t> sizes = {
        64, 128, 256, 512
    };

    constexpr int REPEAT = 5; // average over multiple runs

    for (const size_t size : sizes)
    {
        ScalarField3D<double> field_1(size);
        ScalarField3D<double> field_2(size);

        field_1.fill([](double x, double y, double z)
        {
            const double r = std::sqrt(x * x + y * y + z*z);
            const double theta = std::atan2(y, x);

            return std::sin(8 * std::numbers::pi * r + 4 * theta);
        }, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

        field_2.fill([](double x, double y, double z)
       {
           const double r = std::sqrt(x * x + y * y + z*z);
           const double theta = std::atan2(y, x);

           return std::sin(8 * std::numbers::pi * r + 4 * theta);
       }, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

        long long total_time = 0;
        VectorField3D<double> v_field_1(field_1);
        VectorField3D<double> v_field_2(field_2);

        for (int i = 0; i < REPEAT; ++i)
        {
            auto start = high_resolution_clock::now();
            auto result = v_field_1 + v_field_2;
            auto end = high_resolution_clock::now();

            total_time += duration_cast<milliseconds>(end - start).count();
        }

        const long long avg_time = total_time / REPEAT;

        const size_t total_elements = size * size;

        std::cout << "Size: " << size
                  << " (" << total_elements << " elements)"
                  << " -> " << avg_time << " ms\n";

        file << size << ","
             << total_elements << ","
             << avg_time << "\n";
    }

    file.close();

    std::cout << "\nBenchmark saved to addition_benchmark.csv\n";

    return 0;
}
