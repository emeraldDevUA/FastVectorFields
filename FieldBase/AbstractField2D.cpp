//
// Created by GameRock on 06/01/2026.
//

#include "AbstractField2D.h"

namespace vfFields
{
    template <typename T>
    AbstractField2D<T> AbstractField2D<T>::operator+(const AbstractField2D& field) const
    {
        AbstractField2D newField(this->x_size, this->y_size);
        // Assuming both fields have the same dimensions
        for (size_t i = 0; i < this->x_size; i++)
        {
            T sum;
            for (size_t j = 0; j < this->y_size; ++j)
            {
                sum = this->getValue(i, j) + field.getValue(i, j);
                newField.setValue(i, j, sum);
            }
        }

        return newField;
    }

    template <typename T>
    AbstractField2D<T> AbstractField2D<T>::operator-(const AbstractField2D& field) const
    {
        AbstractField2D newField(this->x_size, this->y_size);
        // Assuming both fields have the same dimensions
        for (size_t i = 0; i < this->x_size; i++)
        {
            T difference;
            for (size_t j = 0; j < this->y_size; ++j)
            {
                difference = this->getValue(i, j) - field.getValue(i, j);
                newField.setValue(i, j, difference);
            }
        }

        return newField;
    }
}
