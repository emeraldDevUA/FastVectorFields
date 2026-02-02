//
// Created by GameRock on 07/01/2026.
//

#ifndef VECTOR2D_H
#define VECTOR2D_H

#include "../NumericalConstants.h"

#include <limits>
#include <cmath>
#include <ostream>

#include <cereal/archives/json.hpp>
#include <cereal/archives/xml.hpp>

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
        {
        }

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

        bool operator==(const Vector2D& other) const
        {
            auto epsilon = std::numeric_limits<T>::epsilon();
            return std::abs(x - other.x) < epsilon
                && std::abs(y - other.y) < epsilon;
        }

        T dot(const Vector2D& other) const
        {
            return x * other.x + y * other.y;
        }

        friend Vector2D operator*(T scalar, const Vector2D& v)
        {
            return {v.x * scalar, v.y * scalar};
        }

        template <class Archive>
        void serialize(Archive& archive)
        {
            archive(
                cereal::make_nvp("x", x),
                cereal::make_nvp("y", y)
            );
        }
    };

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const Vector2D<T>& vector)
    {
        os << "{"
           << "\"x\": " << vector.x
           << ", \"y\": " << vector.y
           << "}";

        return os;
    }

}

#endif //VECTOR2D_H
