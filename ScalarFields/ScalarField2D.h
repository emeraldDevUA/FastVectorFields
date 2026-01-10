//
// Created by GameRock on 09/01/2026.
//

#ifndef SCALARFIELD2D_H
#define SCALARFIELD2D_H

#include "../FieldBase/AbstractField2D.h"
#include "../Vectors/Vector2D.h"


template <typename T>
class ScalarField2D: public AbstractField2D<T> {



public:

    explicit ScalarField2D(const size_t grid_size)
    : AbstractField2D<T>(grid_size)
    {}

    explicit ScalarField2D(size_t x_size, size_t y_size)
        : AbstractField2D<T>(x_size, y_size)
    {}



    void fill(auto&& function, bool center){
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


    vfMath::Vector2D<T> gradient(T x, T y, T epsilon) const{
        double dx = (this->getValue(x + epsilon, y) - this->getValue(x - epsilon, y)) / (2.0 * epsilon);
        double dy = (this->getValue(x, y + epsilon) - this->getValue(x, y - epsilon)) / (2.0 * epsilon);

        return vfMath::Vector2D<T>(static_cast<T>(dx), static_cast<T>(dy));
    }
};



#endif //SCALARFIELD2D_H
