//
// Created by GameRock on 20/01/2026.
//

#ifndef FASTVECTORFIELDS_VECTOR3D_H
#define FASTVECTORFIELDS_VECTOR3D_H

#include <cmath>
#include <ostream>

namespace vfMath
{
    template <typename T>

    class Vector3D
    {
        T x{};
        T y{};
        T z{};

        Vector3D() = default;

        Vector3D(T x, T y, T z) : x(x), y(y), z(z)
        {
        }

        T length() const // ✅ const-correct
        {
            return std::sqrt(x * x + y * y, z * z);
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
    };

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const Vector3D<T>& vector)
    {
        os << "{"
            <<   "x: " << vector.x
            << ", y: " << vector.y
            << ", z: " << vector.z
            << "} ";

        return os;
    }
}
#endif //FASTVECTORFIELDS_VECTOR3D_H
