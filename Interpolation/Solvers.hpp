//
// Created by GameRock on 05/02/2026.
//

#ifndef FASTVECTORFIELDS_SOLVERS_H
#define FASTVECTORFIELDS_SOLVERS_H

#include <vector>
#include <cmath>

// Gaussian elimination (small systems, good enough for terrain/fields)

template <typename T>
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
        if (std::abs(pivot) < 1e-12)
        {
            throw std::runtime_error("Matrix is singular or nearly singular");
        }

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
#endif //FASTVECTORFIELDS_SOLVERS_H
