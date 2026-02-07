//
// Created by GameRock on 23/01/2026.
//

#ifndef FASTVECTORFIELDS_RBFINTERPOLATOR2D_H
#define FASTVECTORFIELDS_RBFINTERPOLATOR2D_H

#include "../Vectors/Vector3D.hpp"
#include "Solvers.hpp"

#include <vector>
#include <cmath>

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

        ~RBFInterpolator2D()
        {
            pts.clear();
            weights.clear();
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

    };
}


#endif //FASTVECTORFIELDS_RBFINTERPOLATOR2D_H
