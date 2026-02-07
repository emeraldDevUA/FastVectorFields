//
// Created by GameRock on 05/02/2026.
//

#ifndef FASTVECTORFIELDS_RBFINTERPOLATOR3D_H
#define FASTVECTORFIELDS_RBFINTERPOLATOR3D_H

#include "../Vectors/Vector4D.hpp"
#include "Solvers.hpp"

#include <vector>
#include <cmath>

namespace vfInterpolation
{
    template <typename T>
    class RBFInterpolator3D
    {
    public:
        using Vec4 = vfMath::Vector4D<T>;

        RBFInterpolator3D(
            const std::vector<Vec4>& samples,
            T epsilon)
            : pts(samples), eps(epsilon)
        {
            computeWeights();
        }

        T evaluate(T x, T y, T z) const
        {
            T result = 0;

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

    private:
        std::vector<Vec4> pts;
        std::vector<T> weights;
        T eps;

        // Inverse multiquadric RBF
        T phi(T r) const
        {
            return T(1) / std::sqrt(r*r + eps*eps);
        }

        static T distance3D(
            T x1, T y1, T z1,
            T x2, T y2, T z2)
        {
            T dx = x1 - x2;
            T dy = y1 - y2;
            T dz = z1 - z2;
            return std::sqrt(dx*dx + dy*dy + dz*dz);
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

        // Naive Gaussian elimination (small N only)

    };
} // vfInterpolation

#endif //FASTVECTORFIELDS_RBFINTERPOLATOR3D_H