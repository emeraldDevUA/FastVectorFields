//
// Created by GameRock on 20/01/2026.
//

#ifndef FASTVECTORFIELDS_VECTOR3D_H
#define FASTVECTORFIELDS_VECTOR3D_H

#include <cmath>
#include <ostream>

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/xml.hpp>

namespace vfMath
{
    template <typename T>

    class Vector3D
    {
    public:
        T x{};
        T y{};
        T z{};

        Vector3D() = default;

        Vector3D(T x, T y, T z) : x(x), y(y), z(z)
        {
        }

        T length() const // ✅ const-correct
        {
            return std::sqrt(x * x + y * y + z * z);
        }

        void normalize()
        {
            T len = length();
            if (len > std::numeric_limits<T>::epsilon()) // ✅ template-safe
            {
                x /= len;
                y /= len;
                z /= len;
            }
        }

        Vector3D operator+(const Vector3D& other) const
        {
            return Vector3D(x + other.x, y + other.y, z + other.z);
        }

        Vector3D operator-(const Vector3D& other) const
        {
            return Vector3D(x - other.x, y - other.y, z - other.z);
        }

        Vector3D operator*(const T multiplier) const
        {
            return Vector3D(multiplier * x, multiplier * y, multiplier * z);
        }

        Vector3D operator/(const T divisor) const
        {
            return Vector3D(x / divisor, y / divisor, z / divisor);
        }

        bool operator==(const Vector3D& other) const
        {
            auto epsilon = std::numeric_limits<T>::epsilon();
            return std::abs(x - other.x) < epsilon
                && std::abs(y - other.y) < epsilon
                && std::abs(z - other.z) < epsilon;
        }

        T dot(const Vector3D& other) const
        {
            return x * other.x + y * other.y + z * other.z;
        }

        friend Vector3D operator*(T scalar, const Vector3D& v)
        {
            return {v.x * scalar, v.y * scalar, v.z * scalar};
        }

        template <class Archive>
        void serialize(Archive& archive)
        {
            archive(
                cereal::make_nvp("x", x),
                cereal::make_nvp("y", y),
                cereal::make_nvp("z", z)
            );
        }
    };

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const Vector3D<T>& vector)
    {
        os << "{"
           << "\"x\": " << vector.x
           << ", \"y\": " << vector.y
           << ", \"z\": " << vector.z
           << "}";

        return os;
    }
};


#endif //FASTVECTORFIELDS_VECTOR3D_H
