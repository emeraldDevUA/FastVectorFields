//
// Created by GameRock on 07/01/2026.
//

#ifndef VECTOR2D_H
#define VECTOR2D_H

#include "../NumericalConstants.h"
#pragma once
#include <limits>
#include <cmath>
#include <ostream>

namespace vfMath
{
    template <typename T>
    class Vector2D
    {
    public:
        T x{};
        T y{};

        Vector2D() = default;

        Vector2D(T x, T y) : x(x), y(y)
        {}

        T length() const // ✅ const-correct
        {
            return std::sqrt(x * x + y * y);
        }

        void normalize()
        {
            T len = length();
            if (len > std::numeric_limits<T>::epsilon()) // ✅ template-safe
            {
                x /= len;
                y /= len;
            }
        }

        Vector2D operator+(const Vector2D& other) const
        {
            return Vector2D(x + other.x, y + other.y);
        }

        Vector2D operator-(const Vector2D& other) const
        {
            return Vector2D(x - other.x, y - other.y);
        }

        Vector2D operator*(const T multiplier) const
        {
            return Vector2D(multiplier * x, multiplier * y);
        }

        Vector2D operator/(const T divisor) const
        {
            return Vector2D(x / divisor, y / divisor);
        }

    };

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const Vector2D<T>& m)
    {
        os << "{" << "x: " << m.x << ", y: " << m.y << "} ";

        return os;
    }
}

#endif //VECTOR2D_H
