//
// Created by GameRock on 05/02/2026.
//

#ifndef FASTVECTORFIELDS_RBFINTERPOLATOR3D_H
#define FASTVECTORFIELDS_RBFINTERPOLATOR3D_H


#include "Solvers.hpp"
#include "../Vectors/Vector4D.hpp"
#include "InterpolationParameters.h"

#include <cmath>
#include <vector>

namespace vfInterpolation
{
    template <typename T>
    class RBFInterpolator3D
    {
    public:
        using Vec4 = vfMath::Vector4D<T>;

        RBFInterpolator3D(
            const std::vector<Vec4>& samples,
            T epsilon, const DistanceFunction distance_function = DistanceFunction::Euclidean, T power = 2)
            : pts(samples), dst_function(distance_function), eps(epsilon), pwr(power)
        {
            computeWeights();
        }

        T evaluate(T x, T y, T z) const
        {
            T result = 0;

            #pragma omp simd reduction(+:result) if (pts.size() > omp_threshold)
            for (size_t i = 0; i < pts.size(); ++i)
            {
                T r = distance3D(
                    x, y, z,
                    pts[i].x, pts[i].y, pts[i].z
                );
                result += weights[i] * phi(r);
            }

            return result;
        }

        ~RBFInterpolator3D()
        {
            pts.clear();
            weights.clear();
        }

    private:
        const size_t omp_threshold = 10000;

        std::vector<Vec4> pts;
        std::vector<T> weights;
        DistanceFunction dst_function;

        T eps;
        T pwr;

        // Inverse multiquadric RBF
        T phi(T r) const
        {
            return T(1) / std::sqrt(r * r + eps * eps);
        }


        T distance3D(
            T x1, T y1, T z1,
            T x2, T y2, T z2
        ) const
        {
            T dx = std::abs(x1 - x2);
            T dy = std::abs(y1 - y2);
            T dz = std::abs(z1 - z2);

            switch (dst_function)
            {
            case DistanceFunction::Euclidean:
                return std::sqrt(dx * dx + dy * dy + dz * dz);

            case DistanceFunction::EuclideanSquared:
                return dx * dx + dy * dy + dz * dz;

            case DistanceFunction::Manhattan:
                return dx + dy + dz;

            case DistanceFunction::Chebyshev:
                return std::max({dx, dy, dz});

            case DistanceFunction::Minkowski:
                return std::pow(std::pow(dx, pwr) + std::pow(dy, pwr)  + std::pow(dz, pwr), T(1) / pwr);
            }

            return 0;
        }

        void computeWeights()
        {
            const size_t N = pts.size();
            weights.resize(N);

            std::vector<std::vector<T>> A(N, std::vector<T>(N));
            std::vector<T> b(N);

            // Build linear system A * weights = b
            for (size_t i = 0; i < N; ++i)
            {
                b[i] = pts[i].w;

                for (size_t j = 0; j < N; ++j)
                {
                    T r = distance3D(
                        pts[i].x, pts[i].y, pts[i].z,
                        pts[j].x, pts[j].y, pts[j].z
                    );

                    A[i][j] = phi(r);
                }
            }

            solveLinearSystem(A, b, weights);
        }
    };
} // vfInterpolation

#endif //FASTVECTORFIELDS_RBFINTERPOLATOR3D_H
