//
// Created by GameRock on 30/01/2026.
//

#ifndef FASTVECTORFIELDS_MULTITHREADEDCOMPUTATIONMODEL_H
#define FASTVECTORFIELDS_MULTITHREADEDCOMPUTATIONMODEL_H
#include "AbstractComputationModel.h"

#ifdef MSVC

#endif


template <typename T>
class MultiThreadedComputationModel : AbstractComputationModel<T>
{
    private:
    int thread_count;


    public:

    MultiThreadedComputationModel(int thread_count): AbstractComputationModel<T>()
    {
        this->thread_count = thread_count;
    };

    AbstractField2D<T> compute(auto lambdaFunction, const AbstractField2D<T>& field1,
                           const AbstractField2D<T>& field2)
    {
        // use threads to compute
    }


};


#endif //FASTVECTORFIELDS_MULTITHREADEDCOMPUTATIONMODEL_H