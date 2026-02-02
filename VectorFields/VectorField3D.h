//
// Created by GameRock on 31/01/2026.
//

#ifndef FASTVECTORFIELDS_VECTORFIELD3D_H
#define FASTVECTORFIELDS_VECTORFIELD3D_H


#include "../Vectors/Vector3D.h"
#include "../ScalarFields/ScalarField3D.h"
#include "../Interpolation/RBFInterpolator2D.h"


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
            : AbstractField3D<Vector3D<T>>(field.getGridSizeX(), field.getGridSizeY(), field.getGridSizeZ())
        {
            for (size_t i = 1; i < field.getGridSizeX() - 1; ++i)
            {
                for (size_t j = 1; j < field.getGridSizeY() - 1 ; ++j)
                {
                    for (size_t k = 1; k < field.getGridSizeY() - 1 ; ++k)
                    {
                        this->setValue(i, j, k, field.gradient(i, j, k));
                    }
                }
            }
        }



        T divergence(size_t i, size_t j, size_t k, T eps = static_cast<T>(1e-6)) const
        {
            const auto& Fx_p = this->getValue(i + 1, j, k);
            const auto& Fx_m = this->getValue(i - 1, j, k);

            const auto& Fy_p = this->getValue(i, j + 1, k);
            const auto& Fy_m = this->getValue(i, j - 1, k);

            const auto& Fz_p = this->getValue(i, j, k + 1);
            const auto& Fz_m = this->getValue(i, j, k - 1);

            T du_dx = (Fx_p.x - Fx_m.x) / (2 * eps);
            T dv_dy = (Fy_p.y - Fy_m.y) / (2 * eps);
            T dw_dz = (Fz_p.z - Fz_m.z) / (2 * eps);

            return du_dx + dv_dy + dw_dz;
        }



        Vector3D<T> curl(size_t i, size_t j, size_t k, T eps = static_cast<T>(1e-6)) const
        {
            const auto& Fx_p = this->getValue(i + 1, j, k);
            const auto& Fx_m = this->getValue(i - 1, j, k);

            const auto& Fy_p = this->getValue(i, j + 1, k);
            const auto& Fy_m = this->getValue(i, j - 1, k);

            const auto& Fz_p = this->getValue(i, j, k + 1);
            const auto& Fz_m = this->getValue(i, j, k - 1);

            T dw_dy = (Fy_p.z - Fy_m.z) / (2 * eps);
            T dv_dz = (Fz_p.y - Fz_m.y) / (2 * eps);

            T du_dz = (Fz_p.x - Fz_m.x) / (2 * eps);
            T dw_dx = (Fx_p.z - Fx_m.z) / (2 * eps);

            T dv_dx = (Fx_p.y - Fx_m.y) / (2 * eps);
            T du_dy = (Fy_p.x - Fy_m.x) / (2 * eps);

            return {
                dw_dy - dv_dz,   // curl.x
                du_dz - dw_dx,   // curl.y
                dv_dx - du_dy    // curl.z
            };
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