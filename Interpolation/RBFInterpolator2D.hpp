//
// Created by GameRock on 23/01/2026.
//

#ifndef FASTVECTORFIELDS_RBFINTERPOLATOR2D_H
#define FASTVECTORFIELDS_RBFINTERPOLATOR2D_H

#include "Solvers.hpp"
#include "../Vectors/Vector3D.hpp"
#include "InterpolationParameters.h"

#include <cmath>
#include <vector>


namespace vfInterpolation
{
    template <typename T>
    class RBFInterpolator2D
    {
    public:
        using Vec3 = vfMath::Vector3D<T>;

        RBFInterpolator2D(const std::vector<Vec3>& points, const T& epsilon,
            const DistanceFunction distance_function = DistanceFunction::Euclidean,
            T power = 2, const RBFKernel kernel = RBFKernel::InverseMultiquadric)
            : pts(points), dst_function(distance_function), rbf_kernel(kernel), eps(epsilon), pwr(power)
        {
            computeWeights();
        }

        T evaluate(T x, T z) const
        {
            T result = 0;

            #pragma omp simd reduction(+:result)
            for (size_t i = 0; i < pts.size(); ++i)
            {
                T r = distance2D(x, z, pts[i].x, pts[i].z);
                result += weights[i] * phi(r);
            }
            return result;
        }

        ~RBFInterpolator2D()
        {
            pts.clear();
            weights.clear();
        }

    private:
        const size_t omp_threshold = 10000;

        std::vector<Vec3> pts;
        std::vector<T> weights;
        DistanceFunction dst_function;
        RBFKernel rbf_kernel;

        T eps;
        T pwr;

        // Inverse Multiquadric RBF kernel
        T phi(T r) const
        {
            return 1.0 / std::sqrt(r * r + eps * eps);
        }

        T distance2D(
            T x1, T y1,
            T x2, T y2
        ) const{
            T dx = std::abs(x1 - x2);
            T dy = std::abs(y1 - y2);

            switch (dst_function)
            {
            case DistanceFunction::Euclidean:
                return std::sqrt(dx*dx + dy*dy);

            case DistanceFunction::EuclideanSquared:
                return dx*dx + dy*dy;

            case DistanceFunction::Manhattan:
                return dx + dy;

            case DistanceFunction::Chebyshev:
                return std::max(dx, dy);

            case DistanceFunction::Minkowski:
                return std::pow(std::pow(dx, pwr) + std::pow(dy, pwr), T(1)/pwr);
            }

            return 0;
        }

        void computeWeights()
        {
            size_t N = pts.size();
            weights.resize(N);

            std::vector<std::vector<T>> A(N, std::vector<T>(N));
            std::vector<T> b(N);

            // Build system A * w = b
            for (size_t i = 0; i < N; ++i)
            {
                b[i] = pts[i].y;
                for (size_t j = 0; j < N; ++j)
                {
                    T r = distance2D(pts[i].x, pts[i].z, pts[j].x, pts[j].z);
                    A[i][j] = phi(r);
                }
            }

            solveLinearSystem(A, b, weights);
        }
    };
}


#endif //FASTVECTORFIELDS_RBFINTERPOLATOR2D_H
