//
// Created by GameRock on 09/01/2026.
//

#include "ScalarField2D.h"

template <typename T>

void ScalarField2D<T>::fill(auto&& function, bool center){
    size_t x_size = this->x_size;
    size_t y_size = this->y_size;

    for (size_t i = 0; i < x_size; ++i) {
        for (size_t j = 0; j < y_size; ++j) {
            double x_coord = center ? (i + 0.5) / x_size : static_cast<double>(i) / x_size;
            double y_coord = center ? (j + 0.5) / y_size : static_cast<double>(j) / y_size;
            T value = function(x_coord, y_coord);
            this->setValue(i, j, value);
        }
    }
}

template <typename T>
vfMath::Vector2D<T> ScalarField2D<T>::gradient(const T x, const T y, const T epsilon = 10e-6) const
{
    double dx = (this->getValue(x + epsilon, y) - this->getValue(x - epsilon, y)) / (2.0 * epsilon);
    double dy = (this->getValue(x, y + epsilon) - this->getValue(x, y - epsilon)) / (2.0 * epsilon);

    return vfMath::Vector2D<T>(static_cast<T>(dx), static_cast<T>(dy));
}