//
// Created by GameRock on 16/01/2026.
//

#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include "../Vectors/Vector2D.h"
#include "../Vectors/Vector3D.h"

#include <vector>

using std::vector;
using vfMath::Vector3D;

template <typename T>
class Interpolation
{
public:
    static T linear(T v0, T v1, double t)
    {
        return static_cast<T>((1.0 - t) * v0 + t * v1);
    }


    // has to use vector 3d IG?
    // y = f(x,z) -> therefore the output is the y component
    // I will use it this way for now:
    // Pi: {Xi, cos(phi), Zi}  where Yi = f(Xi,Zi)
    T lagrangeInterpolation3D(const vfMath::Vector2D<T>& P0, const vector<Vector3D<T>>& referencePoints)
    {
        T sum = 0;
        int length = static_cast<int>(referencePoints.size());

        for (int i = 0; i < length; i++)
        {
            T temp = referencePoints.at(i).y;
            for (int j = 0; j < length - 2; ++j)
            {
                if (i != j)
                {
                    temp = temp * (P0.x * (referencePoints[j + 1].z - referencePoints[j + 2].z) -
                        P0.y * (referencePoints[j + 1].x - referencePoints[j + 2].x) +
                        referencePoints[j + 1].x * referencePoints[j + 2].z -
                        referencePoints[j + 1].z * referencePoints[j + 2].x);

                    temp = temp / (referencePoints[j].x * (referencePoints[j + 1].z - referencePoints[j + 2].z) -
                        referencePoints[j].x * (referencePoints[j + 1].x - referencePoints[j + 2].x) +
                        referencePoints[j + 1].x * referencePoints[j + 2].z -
                        referencePoints[j + 1].z * referencePoints[j + 2].x);
                }
            }
            sum += temp;
        }

        return sum;
    }
};


#endif //INTERPOLATION_H
