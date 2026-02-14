//
// Created by GameRock on 06/02/2026.
//

#ifndef FASTVECTORFIELDS_VECTOR4D_H
#define FASTVECTORFIELDS_VECTOR4D_H

#include <cmath>
#include <ostream>

#include <cereal/archives/json.hpp>

namespace vfMath
{
    template <typename T>

    class Vector4D
    {
    public:
        T x{};
        T y{};
        T z{};
        T w{};

        Vector4D() = default;

        Vector4D(T x, T y, T z, T w) : x(x), y(y), z(z), w(w)
        {
        }

        T length() const // ✅ const-correct
        {
            return std::sqrt(x * x + y * y + z * z + w * w);
        }

        void normalize(T eps = static_cast<T>(1e-9))
        {
            T len = length();
            if (len > eps) // ✅ template-safe
            {
                x /= len;
                y /= len;
                z /= len;
                w /= len;
            }
        }

        Vector4D operator+(const Vector4D& other) const
        {
            return Vector4D(x + other.x, y + other.y, z + other.z, w + other.w);
        }

        Vector4D operator-(const Vector4D& other) const
        {
            return Vector4D(x - other.x, y - other.y, z - other.z, w - other.w);
        }

        Vector4D operator*(const T multiplier) const
        {
            return Vector4D(multiplier * x, multiplier * y, multiplier * z, multiplier * w);
        }

        Vector4D operator/(const T divisor) const
        {
            return Vector4D(x / divisor, y / divisor, z / divisor, w / divisor);
        }

        bool operator==(const Vector4D& other) const
        {
            auto epsilon = 1e-12;
            return std::abs(x - other.x) < epsilon
                && std::abs(y - other.y) < epsilon
                && std::abs(z - other.z) < epsilon
                && std::abs(w - other.w) < epsilon;
        }

        T dot(const Vector4D& other) const
        {
            return x * other.x + y * other.y + z * other.z + w * other.w;
        }

        friend Vector4D operator*(T scalar, const Vector4D& v)
        {
            return {v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar};
        }

        template <class Archive>
        void serialize(Archive& archive)
        {
            archive(
                cereal::make_nvp("x", x),
                cereal::make_nvp("y", y),
                cereal::make_nvp("z", z),
                cereal::make_nvp("w", w)
            );
        }
    };

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const Vector4D<T>& vector)
    {
        os << "{"
            << "\"x\": " << vector.x
            << ", \"y\": " << vector.y
            << ", \"z\": " << vector.z
            << ", \"w\": " << vector.w
            << "}";

        return os;
    }
};


#endif //FASTVECTORFIELDS_VECTOR4D_H
