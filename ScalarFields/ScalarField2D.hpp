//
// Created by GameRock on 09/01/2026.
//

#ifndef SCALARFIELD2D_H
#define SCALARFIELD2D_H

#include "../Vectors/Vector2D.hpp"
#include "../FieldBase/AbstractField2D.hpp"

using vfMath::Vector2D;

namespace vfFields
{
    template <typename T>
    class ScalarField2D : public AbstractField2D<T>
    {
    public:
        explicit ScalarField2D(const size_t grid_size)
            : AbstractField2D<T>(grid_size)
        {
        }

        explicit ScalarField2D(size_t x_size, size_t y_size)
            : AbstractField2D<T>(x_size, y_size)
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


        Vector2D<T> gradient(T x, T y, T epsilon) const
        {
            // other derivative type
            double dx = (this->getValue(x + epsilon, y) - this->getValue(x - epsilon, y)) / (2.0 * epsilon);
            double dy = (this->getValue(x, y + epsilon) - this->getValue(x, y - epsilon)) / (2.0 * epsilon);

            return Vector2D<T>(static_cast<T>(dx), static_cast<T>(dy));
        }

        ScalarField2D operator+(const ScalarField2D& field) const
        {
            auto row_size = this->x_size;
            auto column_size = this->y_size;

            ScalarField2D newField(row_size, column_size);
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

        ScalarField2D operator-(const ScalarField2D& field) const
        {
            auto row_size = this->x_size;
            auto column_size = this->y_size;

            ScalarField2D newField(row_size, column_size);
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
#endif //SCALARFIELD2D_H
