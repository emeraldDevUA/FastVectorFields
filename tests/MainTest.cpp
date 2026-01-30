#include <iostream>
#include <fstream>
#include <cmath>

#include "../VectorFields/VectorField2D.h"
#include "../ScalarFields/ScalarField2D.h"
#include "../Vectors/Vector2D.h"
#include  "../Vectors/Vector3D.h"

#include  "../Interpolation/RBFInterpolator2D.h"
#include "../ComputationModels/MultiThreadedComputationModel.h"
#include <cereal/types/memory.hpp>

using namespace vfMath;

#define m_PI 3.14159265358979323846


int main()
{

    auto size_x = 50;
    auto size_y = 50;

    ScalarField2D<double> scalar_field(size_x, size_y);

    scalar_field.fill([](double x, double y)
    {
         const double r = std::sqrt(x*x + y*y);
         const double theta = std::atan2(y, x);

         return std::sin(8 * m_PI * r + 4 * theta);
    }, -1.0, 1.0, -1.0, 1.0);

    // std::cout << scalar_field << std::endl;

    VectorField2D vector_field(scalar_field);
    // vector_field.normalize();

    std::ofstream os1("scalar_field.json", std::ios::binary);
    cereal::JSONOutputArchive archive1(os1);

    archive1(cereal::make_nvp<>("scalar_field", scalar_field));

    VectorField2D vortex(
    size_x, size_y,
        [](double x, double y) { return  x + y; },
        [](double x, double y) { return  y - x; },
        -1.0, 1.0, -1.0, 1.0
    );


    MultiThreadedComputationModel<double> model(5);

    auto result = model.compute(
    [](size_t i, size_t j,
       const auto& f1,
       const auto& f2)
    {
        return f1(i, j) + f2(i, j);
    },
    vector_field,
    vortex
    );

    std::ofstream os("vector_field.json", std::ios::binary);
    cereal::JSONOutputArchive archive(os);
    archive(cereal::make_nvp<>("vector_field", result));

    return 0;
}
