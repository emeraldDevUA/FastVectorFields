//
// Created by GameRock on 09/01/2026.
//

#ifndef SCALARFIELD2D_H
#define SCALARFIELD2D_H

#include "../FieldBase/AbstractField2D.h"
#include "../Vectors/Vector2D.h"


template <typename T>
class ScalarField2D: public AbstractField2D<T> {


    explicit ScalarField2D(const size_t grid_size)
    : AbstractField2D<T>(grid_size)
    {}

    ScalarField2D(size_t x_size, size_t y_size)
        : AbstractField2D<T>(x_size, y_size)
    {}



    void fill(auto&& function, bool center);

    vfMath::Vector2D<T> gradient(const T x, const T y, const T epsilon) const;
};



#endif //SCALARFIELD2D_H
