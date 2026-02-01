//
// Created by GameRock on 31/01/2026.
//

#ifndef FASTVECTORFIELDS_SCALARFIELD3D_H
#define FASTVECTORFIELDS_SCALARFIELD3D_H

#include "../FieldBase/AbstractField3D.h"
#include "../Vectors/Vector3D.h"

using vfMath::Vector3D;

namespace vfFields
{
    template <typename T>
    class ScalarField3D : public AbstractField3D<T>
    {
    public:
        explicit ScalarField3D(const size_t grid_size)
            : AbstractField3D<T>(grid_size)
        {
        }

        explicit ScalarField3D(size_t x_size, size_t y_size)
            : AbstractField3D<T>(x_size, y_size)
        {
        }

        template <typename Function>
        void fill(Function&& function, T x0, T x1, T y0, T y1)
        {
            const size_t x_local = this->x_size;
            const size_t y_local = this->y_size;

            T delta_x = (x1 - x0) / static_cast<T>(x_local);
            T delta_y = (y1 - y0) / static_cast<T>(y_local);

            for (size_t i = 0; i < x_local; ++i)
            {
                for (size_t j = 0; j < y_local; ++j)
                {
                    double x_coord = x0 + i * delta_x;
                    double y_coord = y0 + j * delta_y;

                    T value = function(x_coord, y_coord);
                    this->setValue(i, j, value);
                }
            }
        }


        Vector3D<T> gradient(T x, T y, T z, T epsilon) const
        {
            // other derivative type
            double dx = (this->getValue(x + epsilon, y, z) - this->getValue(x - epsilon, y, z)) / (2.0 * epsilon);
            double dy = (this->getValue(x, y + epsilon, z) - this->getValue(x, y - epsilon, z)) / (2.0 * epsilon);
            double dz = (this->getValue(x, y, z + epsilon) - this->getValue(x, y, z - epsilon)) / (2.0 * epsilon);

            return Vector3D<T>(static_cast<T>(dx), static_cast<T>(dy), static_cast<T>(dz));
        }
    };
}
#endif //FASTVECTORFIELDS_SCALARFIELD3D_H
