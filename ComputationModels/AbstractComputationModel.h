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


    AbstractField2D<T> compute(auto lambdaFunction, const AbstractField2D<T>& field1,
                               const AbstractField2D<T>& field2)
    {
        size_t field_x_1 = field1.getGridSizeX();
        size_t field_y_1 = field1.getGridSizeX();

        size_t field_x_2 = field2.getGridSizeX();
        size_t field_y_2 = field2.getGridSizeX();

        size_t final_x_size = field_x_1 < field_x_2 ? field_x_1 : field_x_2;
        size_t final_y_size = field_y_1 < field_y_2 ? field_y_1 : field_y_2;

        AbstractField2D<T> new_field(final_x_size, final_y_size);

        for (size_t x = 0; x < final_x_size; x++)
        {
            for (size_t y = 0; y < final_y_size; y++)
            {
                new_field[x][y] = lambdaFunction(field1.getValue(x, y), field2.getValue(x, y));
            }
        }

        return new_field;
    }
};


#endif //FASTVECTORFIELDS_ABSTRACTCOMPUTATIONMODEL_H
