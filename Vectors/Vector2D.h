//
// Created by GameRock on 07/01/2026.
//

#ifndef VECTOR2D_H
#define VECTOR2D_H

#include "../NumericalConstants.h"
#pragma once
#include <limits>
#include "cmath"
namespace vfMath
{
    template <typename T>
    class Vector2D
    {
    public:
        T x{};
        T y{};

        Vector2D() = default;                // ✅ REQUIRED
        Vector2D(T x, T y) : x(x), y(y) {}

        T length() const                     // ✅ const-correct
        {
            return std::sqrt(x * x + y * y);
        }

        void normalize()
        {
            T len = length();
            if (len > std::numeric_limits<T>::epsilon())   // ✅ template-safe
            {
                x /= len;
                y /= len;
            }
        }
    };
}


#endif //VECTOR2D_H
