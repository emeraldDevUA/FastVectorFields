//
// Created by GameRock on 10/03/2026.
//


#include "../VectorFields/VectorField3D.hpp"

#include <iostream>
#include <fstream>
#include <cereal/types/memory.hpp>
#include <chrono>

using vfFields::VectorField3D;

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

    auto Px = [](double x, double y, double z)
    {
        double vortex = -y / (1.0 + x*x + y*y);
        double flow   = std::sin(0.1 * z);

        return vortex + flow;
    };

    auto Py = [](double x, double y, double z)
    {
        double vortex =  x / (1.0 + x*x + y*y);
        double flow   = 0.2 * std::cos(0.1 * z);

        return vortex + flow;
    };

    auto Pz = [](double x, double y, double z)
    {
        return 0.3 * std::sin(0.15 * x) * std::cos(0.15 * y);
    };


    VectorField3D<double> vector_field_3d(
        32, 32, 16,
        Px, Py, Pz,
        -5.0, 5.0,
        -5.0, 5.0,
        -5.0, 5.0
    );
    serializeToJson(vector_field_3d, "vector_field_wind.json", "vector_field");


    return EXIT_SUCCESS;
}