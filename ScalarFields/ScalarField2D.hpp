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


        Vector2D<T> gradient(T x, T y, T epsilon = static_cast<T>(1)) const
        {
            // other derivative type
            double dx = (this->getValue(x + epsilon, y) - this->getValue(x - epsilon, y)) / (2.0 * epsilon);
            double dy = (this->getValue(x, y + epsilon) - this->getValue(x, y - epsilon)) / (2.0 * epsilon);

            return Vector2D<T>(static_cast<T>(dx), static_cast<T>(dy));
        }

        ScalarField2D operator+(const ScalarField2D& field) const
        {
            if (!(this->x_size == field.x_size && this->y_size == field.y_size))
            {
                throw std::out_of_range("Field dimensions don't match for addition.");
            }

            const size_t row_size = field.x_size;
            const size_t column_size = field.y_size;

            ScalarField2D newField(row_size, column_size);
            const size_t full_size = row_size * column_size;

            for (size_t i = 0; i < full_size; ++i)
                newField.inner_data[i] = this->inner_data[i] + field.inner_data[i];

            return newField;
        }

        ScalarField2D operator-(const ScalarField2D& field) const
        {
            if (!(this->x_size == field.x_size && this->y_size == field.y_size))
            {
                throw std::out_of_range("Field dimensions don't match for subtraction.");
            }

            const size_t row_size = field.x_size;
            const size_t column_size = field.y_size;

            ScalarField2D newField(row_size, column_size);
            const size_t full_size = row_size * column_size;

            for (size_t i = 0; i < full_size; ++i)
                newField.inner_data[i] = this->inner_data[i] - field.inner_data[i];

            return newField;
        }
    };
}
#endif //SCALARFIELD2D_H
