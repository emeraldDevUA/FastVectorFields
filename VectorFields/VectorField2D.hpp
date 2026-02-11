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
            for (size_t i = 1; i + 1 < field.getGridSizeX(); ++i)
            {
                for (size_t j = 1; j + 1 < field.getGridSizeY(); ++j)
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

        T divergence(size_t i, size_t j, T eps = static_cast<T>(1e-6)) const
        {
            // Assuming your class provides access to u(i,j) and v(i,j)
            // You might need to adapt these if your storage is different

            //assume that u(i, j) is this(i, j).x and v(i, j) is this(i, j).y

            T du_dx = (this->getValue(i + 1, j).x - this->getValue(i - 1, j).x) / (2.0 * eps);
            T dv_dy = (this->getValue(i, j + 1).y - this->getValue(i, j - 1).y) / (2.0 * eps);

            return du_dx + dv_dy;
        }


        T curl(size_t i, size_t j, T eps = static_cast<T>(1e-6)) const
        {
            // For 2D vector field (u, v), curl = dv/dx - du/dy
            T du_dx = (this->getValue(i + 1, j).x - this->getValue(i - 1, j).x) / (2.0 * eps);
            T dv_dy = (this->getValue(i, j + 1).y - this->getValue(i, j - 1).y) / (2.0 * eps);

            // Return as Vector2D with curl value in z-component represented as x, y=0
            // Or if you want the scalar curl, consider changing return type to T
            return du_dx - dv_dy;
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
                    value.normalize();

                    this->setValue(i, j, value);
                }
            }
        }


        void normalize()
        {
            const size_t full_size = this->getGridSizeX() * this->getGridSizeY();
            for (size_t i = 0; i < full_size; ++i)
            {
                this->inner_data[i].normalize();
            }
        }

        VectorField2D operator+(const VectorField2D& field) const
        {
            auto row_size = this->x_size;
            auto column_size = this->y_size;

            VectorField2D newField(row_size, column_size);
            // Assuming both fields have the same dimensions
            for (size_t i = 0; i < row_size; i++)
            {
                for (size_t j = 0; j < column_size; ++j)
                {
                    newField.inner_data[i * row_size + j] =
                        this->inner_data[i * row_size + j] + field.inner_data[i * row_size + j];
                }
            }

            return newField;
        }

        VectorField2D operator-(const VectorField2D& field) const
        {
            auto row_size = this->x_size;
            auto column_size = this->y_size;

            VectorField2D newField(row_size, column_size);
            // Assuming both fields have the same dimensions
            for (size_t i = 0; i < row_size; i++)
            {
                for (size_t j = 0; j < column_size; ++j)
                {
                    newField.inner_data[i * row_size + j] =
                        this->inner_data[i * row_size + j] - field.inner_data[i * row_size + j];
                }
            }

            return newField;
        }
    };
}

#endif //VECTORFIELD2D_H
