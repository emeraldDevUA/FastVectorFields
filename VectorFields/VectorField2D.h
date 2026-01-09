//
// Created by GameRock on 07/01/2026.
//

#ifndef VECTORFIELD2D_H
#define VECTORFIELD2D_H

#include "../FieldBase/AbstractField2D.h"
#include "../ScalarFields/ScalarField2D.h"

template <typename T>
class VectorField2D : public AbstractField2D<T>
{


public:


    explicit VectorField2D(const size_t grid_size)
        : AbstractField2D<T>(grid_size)
    {}

    VectorField2D(size_t x_size, size_t y_size)
        : AbstractField2D<T>(x_size, y_size)
    {}

    VectorField2D(const ScalarField2D<T>& field)
    {
        // compute grads
    }


    [[nodiscard]] double divergence(const double x, const double y) const
    {
        // compute divergence

        return 0.0f;
    }




};



#endif //VECTORFIELD2D_H
