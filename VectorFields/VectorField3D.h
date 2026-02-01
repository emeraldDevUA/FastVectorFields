//
// Created by GameRock on 31/01/2026.
//

#ifndef FASTVECTORFIELDS_VECTORFIELD3D_H
#define FASTVECTORFIELDS_VECTORFIELD3D_H

#include "../ScalarFields/ScalarField3D.h"
#include  "../Vectors/Vector3D.h"
#include  "../Interpolation/RBFInterpolator2D.h"


using vfMath::Vector3D;
using vfInterpolation::RBFInterpolator2D;


namespace vfFields
{
    template <typename T>
    class VectorField3D : public AbstractField3D<Vector3D<T>>
    {
    public:
        explicit VectorField3D(size_t grid_size)
            : AbstractField3D<Vector3D<T>>(grid_size)
        {
        }

        VectorField3D(size_t x_size, size_t y_size)
            : AbstractField3D<Vector3D<T>>(x_size, y_size)
        {
        }

        explicit VectorField3D(const ScalarField3D<T>& field)
            : AbstractField3D<Vector3D<T>>(field.getGridSizeX(), field.getGridSizeY())
        {
            for (size_t i = 1; i < field.getGridSizeX() - 1; ++i)
            {
                for (size_t j = 1; j < field.getGridSizeY() - 1 ; ++j)
                {
                    for (size_t k = 1; k < field.getGridSizeY() - 1 ; ++k)
                    {
                        this->setValue(i, j, k, field.gradient(i, j, 1));
                    }
                }
            }
        }



        T divergence(size_t i, size_t j, size_t k, T eps = 1e-6) const
        {
            // Assuming your class provides access to u(i,j) and v(i,j)
            // You might need to adapt these if your storage is different

            //assume that u(i, j) is this(i, j).x and v(i, j) is this(i, j).y

            T du_dx = (this->getValue(i + 1, j).x - this->getValue(i - 1, j).x) / (2.0 * eps);
            T dv_dy = (this->getValue(i, j + 1).y - this->getValue(i, j - 1).y) / (2.0 * eps);

            return du_dx + dv_dy;
        }


        T curl(size_t i, size_t j, size_t k, T eps = 1e-6) const
        {
            // For 2D vector field (u, v), curl = dv/dx - du/dy
            T dv_dx = (this->getValue(i + 1, j).x - this->getValue(i - 1, j).x) / (2.0 * eps);
            T du_dy = (this->getValue(i, j + 1).y - this->getValue(i, j - 1).y) / (2.0 * eps);

            // Return as Vector2D with curl value in z-component represented as x, y=0
            // Or if you want the scalar curl, consider changing return type to T
            return dv_dx - du_dy;
        }


        void normalize()
        {
            const size_t full_size = this->getGridSizeX() * this->getGridSizeY() * this->getGridSizeZ();
            for (size_t i = 0; i < full_size; ++i)
            {
                this->inner_data[i].normalize();
            }
        }
    };
} // vfFields

#endif //FASTVECTORFIELDS_VECTORFIELD3D_H