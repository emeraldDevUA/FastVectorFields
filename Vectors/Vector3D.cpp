//
// Created by GameRock on 20/01/2026.
//

#include "Vector3D.h"

namespace vfMath
{
    template <typename T>
    std::ostream& operator<<(std::ostream& os, const Vector3D<T>& vector)
    {
        os << "{"
            << "x: " << vector.x
            << ", y: " << vector.y
            << ", z: " << vector.z
            << "} ";

        return os;
    }
}
