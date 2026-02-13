//
// Created by GameRock on 31/01/2026.
//

#ifndef FASTVECTORFIELDS_VECTORFIELD3D_H
#define FASTVECTORFIELDS_VECTORFIELD3D_H


#include "../Vectors/Vector3D.hpp"
#include "../Vectors/Vector4D.hpp"
#include "../ScalarFields/ScalarField3D.hpp"
#include "../Interpolation/RBFInterpolator3D.hpp"


using vfMath::Vector3D;
using vfMath::Vector4D;
using vfInterpolation::RBFInterpolator3D;


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

        VectorField3D(size_t x_size, size_t y_size, size_t z_size)
            : AbstractField3D<Vector3D<T>>(x_size, y_size, z_size)
        {
        }

        explicit VectorField3D(const ScalarField3D<T>& field)
            : AbstractField3D<Vector3D<T>>(field.getGridSizeX(), field.getGridSizeY(), field.getGridSizeZ())
        {
            for (size_t i = 1; i < field.getGridSizeX() - 1; ++i)
            {
                for (size_t j = 1; j < field.getGridSizeY() - 1; ++j)
                {
                    for (size_t k = 1; k < field.getGridSizeZ() - 1; ++k)
                    {
                        this->setValue(i, j, k, field.gradient(i, j, k, 1));
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
                dw_dy - dv_dz, // curl.x
                du_dz - dw_dx, // curl.y
                dv_dx - du_dy // curl.z
            };
        }

        void fillWithInterpolation(const T empty_point_threshold = static_cast<T>(1e-6),
                                   const T rbf_epsilon = static_cast<T>(0.8))
        {
            auto row_size = this->x_size;
            auto column_size = this->y_size;
            auto matrix_depth = this->z_size;


            std::vector<Vector4D<T>> xValues;
            std::vector<Vector4D<T>> yValues;
            std::vector<Vector4D<T>> zValues;

            for (size_t k = 0; k < matrix_depth; ++k)
            {
                for (size_t i = 0; i < row_size; ++i)
                {
                    for (size_t j = 0; j < column_size; ++j)
                    {
                        if (auto v = this->getValue(i, j, k); v.length() > empty_point_threshold)
                        {
                            auto x_coord = static_cast<T>(i);
                            auto y_coord = static_cast<T>(j);
                            auto z_coord = static_cast<T>(k);

                            xValues.push_back({x_coord, y_coord, z_coord, v.x});
                            yValues.push_back({x_coord, y_coord, z_coord, v.y});
                            zValues.push_back({x_coord, y_coord, z_coord, v.z});
                        }
                    }
                }
            }

            if (xValues.size() < 3)
            {
                throw std::logic_error("RBF interpolation does not work with less then 3 non-empty points.");
            }

            RBFInterpolator3D<T> xInterpolator(xValues, rbf_epsilon);
            RBFInterpolator3D<T> yInterpolator(yValues, rbf_epsilon);
            RBFInterpolator3D<T> zInterpolator(zValues, rbf_epsilon);

            for (size_t k = 0; k < matrix_depth; ++k)
            {
                for (size_t i = 0; i < row_size; ++i)
                {
                    for (size_t j = 0; j < column_size; ++j)
                    {
                        auto x_coord = static_cast<T>(i);
                        auto y_coord = static_cast<T>(j);
                        auto z_coord = static_cast<T>(k);

                        T vectorX = xInterpolator.evaluate(x_coord, y_coord, z_coord);
                        T vectorY = yInterpolator.evaluate(x_coord, y_coord, z_coord);
                        T vectorZ = zInterpolator.evaluate(x_coord, y_coord, z_coord);

                        Vector3D<T> value(vectorX, vectorY, vectorZ);
                        value.normalize();

                        this->setValue(i, j, k, value);
                    }
                }
            }
        }


        void normalize()
        {
            for (auto& v : this->inner_data)
                v.normalize();
        }

        VectorField3D operator+(const VectorField3D& field) const
        {
            auto row_size = this->x_size;
            auto column_size = this->y_size;
            auto slice_depth = this->z_size;

            VectorField3D newField(row_size, column_size, slice_depth);
            // Assuming both fields have the same dimensions
            for (size_t i = 0; i < row_size; i++)
            {
                for (size_t j = 0; j < column_size; ++j)
                {
                    for (size_t k = 0; k < slice_depth; ++k)
                    {
                        size_t index = i * column_size + j + (row_size * column_size) * k;

                        newField.inner_data[index] =
                            this->inner_data[index] + field.inner_data[index];
                    }
                }
            }

            return newField;
        }


        VectorField3D operator-(const VectorField3D& field) const
        {
            auto row_size = this->x_size;
            auto column_size = this->y_size;
            auto slice_depth = this->z_size;

            VectorField3D newField(row_size, column_size, slice_depth);
            // Assuming both fields have the same dimensions
            for (size_t i = 0; i < row_size; i++)
            {
                for (size_t j = 0; j < column_size; ++j)
                {
                    for (size_t k = 0; k < slice_depth; ++k)
                    {
                        size_t index = i * column_size + j + (row_size * column_size) * k;

                        newField.inner_data[index] =
                            this->inner_data[index] - field.inner_data[index];
                    }
                }
            }

            return newField;
        }

    };
} // vfFields

#endif //FASTVECTORFIELDS_VECTORFIELD3D_H
