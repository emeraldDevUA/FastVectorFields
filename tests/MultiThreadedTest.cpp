#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include <cmath>


#include "../ComputationModels/MultiThreadedComputationModel.h"
#include "../VectorFields/VectorField2D.h"

static VectorField2D<double> make_field(size_t N, size_t M, double seed)
{
    VectorField2D<double> field(N, M);

    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < M; ++j)
        {
            double x = seed + i + j;
            for (int k = 0; k < 50; ++k)
                x = std::sin(x) + std::cos(x);
            field.setValue(i, j, Vector2D(x, x));
        }

    return field;
}

static VectorField2D<double>
compute_single_threaded(
    const VectorField2D<double>& f1,
    const VectorField2D<double>& f2)
{
    return f1 + f2;
}


TEST_CASE("MultiThreadedComputationModel is faster than single-threaded [+]",
          "[multithread]")
{
    constexpr size_t N = 3000;
    constexpr size_t M = 3000;

    auto field_1 = make_field(N, M, 1.0);
    auto field_2 = make_field(N, M, 2.0);

    MultiThreadedComputationModel<double> model(16);

    // ---- Correctness ----
    auto single = compute_single_threaded(field_1, field_2);

    auto multi = model.compute(
        [](size_t i, size_t j,
           const auto& f1,
           const auto& f2)
        {
            return f1(i, j) + f2(i, j);
        },
        field_1,
        field_2
    );

    REQUIRE(single == multi);

    // ---- Benchmarks ----
    BENCHMARK("Single-threaded +")
    {
        return compute_single_threaded(field_1, field_2);
    };

    BENCHMARK("Multi-threaded +")
    {
        return model.compute(
            [](size_t i, size_t j,
               const auto& f1,
               const auto& f2)
            {
                return f1(i, j) + f2(i, j);
            },
            field_1,
            field_2
        );
    };
}
