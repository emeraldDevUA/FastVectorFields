//
// Created by GameRock on 07/01/2026.
//

#ifndef VECTORFIELD2D_H
#define VECTORFIELD2D_H

#include "../Vectors/Vector3D.hpp"
#include "../ScalarFields/ScalarField2D.hpp"
#include "../Interpolation/RBFInterpolator2D.hpp"


using vfMath::Vector3D;
using vfInterpolation::RBFInterpolator2D;

namespace vfFields
{
    template <typename T>

    class VectorField2D : public AbstractField2D<Vector2D<T>>
    {
    public:
        explicit VectorField2D(size_t grid_size)
            : AbstractField2D<Vector2D<T>>(grid_size)
        {
        }

        VectorField2D(size_t x_size, size_t y_size)
            : AbstractField2D<Vector2D<T>>(x_size, y_size)
        {
        }

        explicit VectorField2D(const ScalarField2D<T>& field)
            : AbstractField2D<Vector2D<T>>(field.getGridSizeX(), field.getGridSizeY())
        {
            const size_t nx = field.getGridSizeX();
            const size_t ny = field.getGridSizeY();

            #pragma omp parallel for collapse(2) if (full_size > this->omp_threshold)
            for (size_t i = 1; i < nx - 1; ++i)
            {
                for (size_t j = 1; j < ny - 1; ++j)
                {
                    this->setValue(i, j, field.gradient(i, j, 1));
                }
            }
        }

        template <typename PxFunc, typename PyFunc>
        explicit VectorField2D(
            size_t x_size,
            size_t y_size,
            PxFunc&& Px,
            PyFunc&& Py,
            T x0, T x1,
            T y0, T y1
        )
            : AbstractField2D<Vector2D<T>>(x_size, y_size)
        {
            const T delta_x = (x1 - x0) / static_cast<T>(x_size);
            const T delta_y = (y1 - y0) / static_cast<T>(y_size);

            #pragma omp parallel for collapse(2) if (full_size > this->omp_threshold)
            for (size_t i = 0; i < x_size; ++i)
            {
                for (size_t j = 0; j < y_size; ++j)
                {
                    const T x = x0 + i * delta_x;
                    const T y = y0 + j * delta_y;

                    this->setValue(i, j, Vector2D<T>{
                                       Px(x, y),
                                       Py(x, y)
                                   });
                }
            }
        }

        T divergence(size_t i, size_t j, T eps = static_cast<T>(1)) const
        {
            //assume that u(i, j) is this(i, j).x and v(i, j) is this(i, j).y
            T du_dx = (this->getValue(i + eps, j).x - this->getValue(i - eps, j).x) / 2.0;
            T dv_dy = (this->getValue(i, j + eps).y - this->getValue(i, j - eps).y) / 2.0;

            return du_dx + dv_dy;
        }


        T curl(size_t i, size_t j, T eps = static_cast<T>(1)) const
        {
            // For 2D vector field (u, v), curl = dv/dx - du/dy
            T du_dy = (this->getValue(i, j + eps).x - this->getValue(i, j - eps).x) / 2.0;
            T dv_dx = (this->getValue(i + eps, j).y - this->getValue(i - eps, j).y) / 2.0;

            return dv_dx - du_dy;
        }


        void fillWithInterpolation(const T empty_point_threshold = static_cast<T>(1e-6),
                                   const T rbf_epsilon = static_cast<T>(0.8))
        {
            auto row_size = this->x_size;
            auto column_size = this->y_size;

            std::vector<Vector3D<T>> xValues;
            std::vector<Vector3D<T>> yValues;

            for (size_t i = 0; i < row_size; ++i)
            {
                for (size_t j = 0; j < column_size; ++j)
                {
                    if (auto v = this->getValue(i, j); v.length() > empty_point_threshold)
                    {
                        auto x_coord = static_cast<T>(i);
                        auto z_coord = static_cast<T>(j);

                        xValues.push_back({x_coord, v.x, z_coord});
                        yValues.push_back({x_coord, v.y, z_coord});
                    }
                }
            }

            if (xValues.size() < 3)
            {
                throw std::logic_error("RBF interpolation does not work with less then 3 non-empty points.");
            }

            RBFInterpolator2D<T> xInterpolator(xValues, rbf_epsilon);
            RBFInterpolator2D<T> yInterpolator(yValues, rbf_epsilon);

            for (size_t i = 0; i < row_size; ++i)
            {
                for (size_t j = 0; j < column_size; ++j)
                {
                    auto x_coord = static_cast<T>(i);
                    auto z_coord = static_cast<T>(j);

                    T vectorX = xInterpolator.evaluate(x_coord, z_coord);
                    T vectorY = yInterpolator.evaluate(x_coord, z_coord);

                    Vector2D<T> value(vectorX, vectorY);

                    this->setValue(i, j, value);
                }
            }
        }


        void normalize(T eps = static_cast<T>(1e-9))
        {
            #pragma omp parallel for
            for (size_t i = 0; i < this->inner_data.size(); ++i)
                this->inner_data[i].normalize(eps);
        }

        VectorField2D operator+(const VectorField2D& field) const
        {
            if (!(this->x_size == field.x_size && this->y_size == field.y_size))
            {
                throw std::out_of_range("Field dimensions don't match for addition.");
            }

            const size_t row_size = field.x_size;
            const size_t column_size = field.y_size;

            VectorField2D newField(row_size, column_size);
            const size_t full_size = row_size * column_size;

            #pragma omp parallel for simd if (full_size > this->omp_threshold)
            for (size_t i = 0; i < full_size; ++i)
                newField.inner_data[i] = this->inner_data[i] + field.inner_data[i];

            return newField;
        }

        VectorField2D operator-(const VectorField2D& field) const
        {
            if (!(this->x_size == field.x_size && this->y_size == field.y_size))
            {
                throw std::out_of_range("Field dimensions don't match for subtraction.");
            }

            const size_t row_size = field.x_size;
            const size_t column_size = field.y_size;

            VectorField2D newField(row_size, column_size);
            const size_t full_size = row_size * column_size;

            #pragma omp parallel for simd if (full_size > this->omp_threshold)
            for (size_t i = 0; i < full_size; ++i)
                newField.inner_data[i] = this->inner_data[i] - field.inner_data[i];

            return newField;
        }
    };
}

#endif //VECTORFIELD2D_H
