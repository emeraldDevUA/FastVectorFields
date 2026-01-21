//
// Created by GameRock on 16/01/2026.
//

#ifndef INTERPOLATION_H
#define INTERPOLATION_H
#include <vector>

#include "../Vectors/Vector2D.h"


using std::pmr::vector;

template <typename T>
class Interpolation
{
    class Vector3D;

public:
    static T linear(T v0, T v1, double t)
    {
        return static_cast<T>((1.0 - t) * v0 + t * v1);
    }


    // has to use vector 3d IG?
    // y = f(x,z) -> therefore the output is the y component
    // I will use it this way for now:
    // Pi: {Xi, cos(phi), Zi}  where Yi = f(Xi,Zi)
    T lagrangeInterpolation3D(const vfMath::Vector2D<T>& P0, const vector<Vector3D>& referencePoints)
    {
        T sum = 0;
        int length = static_cast<int>(referencePoints.size());

        for (int i = 0; i < length; i++)
        {
            T temp = referencePoints.at(i).Y;
            for (int j = 0; j < length - 2; ++j)
            {
                if (i != j)
                {
                    temp = temp * (P0.X * (referencePoints[j + 1].Z - referencePoints[j + 2].Z) -
                        P0.Z * (referencePoints[j + 1].X - referencePoints[j + 2].X) +
                        referencePoints[j + 1].X * referencePoints[j + 2].Z -
                        referencePoints[j + 1].Z * referencePoints[j + 2].X);

                    temp = temp / (referencePoints[j].X * (referencePoints[j + 1].Z - referencePoints[j + 2].Z) -
                        referencePoints[j].X * (referencePoints[j + 1].X - referencePoints[j + 2].X) +
                        referencePoints[j + 1].X * referencePoints[j + 2].Z -
                        referencePoints[j + 1].Z * referencePoints[j + 2].X);
                }
            }
            sum += temp;
        }

        return sum;
    }
};


#endif //INTERPOLATION_H
