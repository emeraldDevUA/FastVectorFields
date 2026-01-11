//
// Created by GameRock on 07/01/2026.
//

#ifndef VECTORFIELD2D_H
#define VECTORFIELD2D_H

#include "../FieldBase/AbstractField2D.h"
#include "../ScalarFields/ScalarField2D.h"

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
        : AbstractField2D<Vector2D<T>>(field.x_size, field.y_size)
    {
        for (size_t i = 1; i + 1 < field.x_size; ++i)
        {
            for (size_t j = 1; j + 1 < field.y_size; ++j)
            {
                this->setValue(i, j, field.gradient(i, j, 1));
            }
        }
    }

    double divergence(size_t i, size_t j) const
    {
        return 0.0;
    }

    template <typename InterpolationMethod>
    void fillWithInterpolation(InterpolationMethod method)
    {
    }
};


#endif //VECTORFIELD2D_H
