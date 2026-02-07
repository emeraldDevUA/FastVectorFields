//
// Created by GameRock on 31/01/2026.
//

#include "AbstractField3D.hpp"


namespace vfFields
{
    template <typename T>
    AbstractField3D<T> AbstractField3D<T>::operator+(const AbstractField3D& field) const
    {
        AbstractField3D newField(this->x_size, this->y_size, this->z_size);
        // Assuming both fields have the same dimensions
        for (size_t i = 0; i < this->x_size; i++)
        {
            T sum;
            for (size_t j = 0; j < this->y_size; ++j)
            {
                for (size_t k = 0; k < this->z_size; ++k)
                {
                    sum = this->getValue(i, j, k) + field.getValue(i, j, k);
                    newField.setValue(i, j, k, sum);
                }
            }
        }

        return newField;
    }


    template <typename T>
    AbstractField3D<T> AbstractField3D<T>::operator-(const AbstractField3D& field) const
    {
        AbstractField3D newField(this->x_size, this->y_size, this->z_size);
        // Assuming both fields have the same dimensions
        for (size_t i = 0; i < this->x_size; i++)
        {
            T difference;
            for (size_t j = 0; j < this->y_size; ++j)
            {
                for (size_t k = 0; k < this->z_size; ++k)
                {
                    difference = this->getValue(i, j, k) - field.getValue(i, j, k);
                    newField.setValue(i, j, k, difference);
                }
            }
        }

        return newField;
    }
}
