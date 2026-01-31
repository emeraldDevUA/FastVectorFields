//
// Created by GameRock on 30/01/2026.
//

#ifndef FASTVECTORFIELDS_MULTITHREADEDCOMPUTATIONMODEL_H
#define FASTVECTORFIELDS_MULTITHREADEDCOMPUTATIONMODEL_H
#include "AbstractComputationModel.h"
#include "../VectorFields/VectorField2D.h"
#include <thread>

using std::jthread;

template <typename T>
class MultiThreadedComputationModel : public AbstractComputationModel<T>
{
    int thread_count;

public:
    explicit MultiThreadedComputationModel(int threads)
        : thread_count(std::max(1, threads)) {}

    template <typename Func>
    AbstractField2D<T> compute(
        Func func,
        const AbstractField2D<T>& field1,
        const AbstractField2D<T>& field2)
    {
        const size_t nx = field1.getGridSizeX();
        const size_t ny = field1.getGridSizeY();

        AbstractField2D<T> result(nx, ny);

        const size_t rows_per_thread = ny / thread_count;
        const size_t remainder = ny % thread_count;

        std::vector<jthread> workers;
        workers.reserve(thread_count);

        size_t y_start = 0;


        auto row_size = this->x_size;
        auto column_size = this->y_size;

        VectorField2D newField(row_size, column_size);
        // Assuming both fields have the same dimensions
        #pragma omp parallel for schedule(static)
        for (size_t i = 0; i < row_size; i++)
        {
            for (size_t j = 0; j < column_size; ++j)
            {
                newField.inner_data[i * row_size + j] =
                   field1.inner_data[i * row_size + j] + field2.inner_data[i * row_size + j];
            }
        }

        return newField;
    }

template <typename Func>
VectorField2D<T> compute(
    Func func,
    const VectorField2D<T>& field1,
    const VectorField2D<T>& field2)
    {
        const size_t nx = field1.getGridSizeX();
        const size_t ny = field1.getGridSizeY();
        //
        // VectorField2D<T> result(nx, ny);
        //
        // const size_t rows_per_thread = ny / thread_count;
        // const size_t remainder = ny % thread_count;
        //
        // std::vector<jthread> workers;
        // workers.reserve(thread_count);
        //
        // size_t y_start = 0;
        //
        // for (int t = 0; t < thread_count; ++t)
        // {
        //     size_t y_end = y_start + rows_per_thread + (t < remainder ? 1 : 0);
        //
        //     workers.emplace_back(
        //         [&, y_start, y_end]()
        //         {
        //             for (size_t j = y_start; j < y_end; ++j)
        //             {
        //                 for (size_t i = 0; i < nx; ++i)
        //                 {
        //                     result.setValue(
        //                         i, j,
        //                         field1.getValue(i, j) + field2.getValue(i, j)
        //                     );
        //                 }
        //             }
        //         }
        //     );
        //
        //     y_start = y_end;
        // }
        //


        // return result; // jthread joins automatically
        VectorField2D<T> newField(nx, ny);
        // Assuming both fields have the same dimensions
        #pragma omp parallel for schedule(static)
        for (int i = 0; i < static_cast<int>(nx * ny); ++i)
        {
            newField.inner_data[i] = field1.inner_data[i] + field2.inner_data[i];
        }

        return newField;

    }
};



#endif //FASTVECTORFIELDS_MULTITHREADEDCOMPUTATIONMODEL_H