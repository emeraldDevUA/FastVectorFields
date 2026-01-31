//
// Created by GameRock on 23/01/2026.
//

#ifndef FASTVECTORFIELDS_RBFINTERPOLATOR2D_H
#define FASTVECTORFIELDS_RBFINTERPOLATOR2D_H

#include "../Vectors/Vector3D.h"

#include <vector>
#include <cmath>
#include <cassert>
namespace vfInterpolation
{
    template <typename T>
    class RBFInterpolator2D
    {
    public:
        using Vec3 = vfMath::Vector3D<T>;

        RBFInterpolator2D(const std::vector<Vec3>& points, const T& epsilon)
            : pts(points), eps(epsilon)
        {
            computeWeights();
        }

        T evaluate(T x, T z) const
        {
            T result = 0;
            for (size_t i = 0; i < pts.size(); ++i)
            {
                T r = distance2D(x, z, pts[i].x, pts[i].z);
                result += weights[i] * phi(r);
            }
            return result;
        }

    private:
        std::vector<Vec3> pts;
        std::vector<T> weights;
        T eps;

        // Inverse Multiquadric RBF kernel
        T phi(T r) const
        {
            return 1.0 / std::sqrt(r * r + eps * eps);
        }

        static T distance2D(T x1, T z1, T x2, T z2)
        {
            T dx = x1 - x2;
            T dz = z1 - z2;
            return std::sqrt(dx * dx + dz * dz);
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

        // Gaussian elimination (small systems, good enough for terrain/fields)
        void solveLinearSystem(
            std::vector<std::vector<T>>& A,
            std::vector<T>& b,
            std::vector<T>& x)
        {
            const size_t N = b.size();

            for (size_t i = 0; i < N; ++i)
            {
                // Pivot
                T pivot = A[i][i];
                assert(std::abs(pivot) > 1e-12);

                for (size_t j = i; j < N; ++j)
                    A[i][j] /= pivot;
                b[i] /= pivot;

                // Eliminate
                for (size_t k = 0; k < N; ++k)
                {
                    if (k == i) continue;
                    T factor = A[k][i];
                    for (size_t j = i; j < N; ++j)
                        A[k][j] -= factor * A[i][j];
                    b[k] -= factor * b[i];
                }
            }

            x = b;
        }
    };
}


#endif //FASTVECTORFIELDS_RBFINTERPOLATOR2D_H
