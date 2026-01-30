//
// Created by GameRock on 29/01/2026.
//

#ifndef FASTVECTORFIELDS_ABSTRACTCOMPUTATIONMODEL_H
#define FASTVECTORFIELDS_ABSTRACTCOMPUTATIONMODEL_H
#include "../FieldBase/AbstractField2D.h"


template <typename T>

class AbstractComputationModel
{

public:
    AbstractComputationModel()
    = default;

    //
    AbstractField2D<T> compute(auto lambdaFunction, const AbstractField2D<T> &field1,
        const AbstractField2D<T> &field_2)
    {
        size_t field_x = field1.getGridSizeX();
        size_t field_y = field1.getGridSizeX();

        AbstractField2D<T> new_field(field_x, field_y);

        for (size_t x = 0; x < field_x; x++)
        {
            for (size_t y = 0; y < field_y; y++)
            {

                new_field[x][y] = lambdaFunction(field1.getValue(x, y), field_2.getValue(x, y));
            }
        }

        return new_field;
    }

};


#endif //FASTVECTORFIELDS_ABSTRACTCOMPUTATIONMODEL_H