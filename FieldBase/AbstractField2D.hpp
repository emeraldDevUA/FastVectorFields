//
// Created by GameRock on 06/01/2026.
//

#ifndef ABSTRACTFIELD_H
#define ABSTRACTFIELD_H

#include <vector>
#include <ostream>
#include <cmath>

#include <cereal/types/vector.hpp>


namespace vfFields
{
    template <typename T>

    class AbstractField2D
    {
    protected:
        std::vector<T> inner_data;
        size_t x_size;
        size_t y_size;

        size_t omp_threshold = 10000;

    public:
        explicit AbstractField2D(const size_t grid_size)
            : inner_data(grid_size * grid_size),
              x_size(grid_size),
              y_size(grid_size)
        {
        }

        AbstractField2D(const size_t x_size, const size_t y_size)
            : inner_data(x_size * y_size),
              x_size(x_size),
              y_size(y_size)
        {
        }


        void setValue(const size_t x, const size_t y, const T value)
        {
            inner_data[x * y_size + y] = value;
        }

        T getValue(const size_t x, const size_t y) const
        {
            return inner_data[x * y_size + y];
        }

        void setOmpThreshold(const size_t threshold)
        {
            this->omp_threshold = threshold;
        }

        [[nodiscard]] size_t getGridSizeX() const { return x_size; }
        [[nodiscard]] size_t getGridSizeY() const { return y_size; }


        T sample(double x, double y) const;

        T& operator()(size_t x, size_t y);

        bool operator==(const AbstractField2D& field) const
        {
            if (!(this->x_size == field.x_size && this->y_size == field.y_size))
            {
                return false;
            }

            const size_t full_size = this->getGridSizeX() * this->getGridSizeY();
            for (size_t i = 0; i < full_size; ++i)
            {
                if (this->inner_data[i] != field.inner_data[i])
                {
                    return false;
                }
            }

            return true;
        }


        template <class Archive>
        void serialize(Archive& archive)
        {
            archive(
                cereal::make_nvp("x_size", this->x_size),
                cereal::make_nvp("y_size", this->y_size),
                cereal::make_nvp("inner_data", this->inner_data)
            );
        }
    };


    template <typename T>
    std::ostream& operator<<(std::ostream& os, const AbstractField2D<T>& m)
    {
        os << "[\n";

        for (size_t i = 0; i < m.getGridSizeY(); ++i)
        {
            os << "  [";

            for (size_t j = 0; j < m.getGridSizeX(); ++j)
            {
                os << m.getValue(i, j);
                if (j + 1 < m.getGridSizeX())
                    os << ", ";
            }

            os << "]";
            if (i + 1 < m.getGridSizeY())
                os << ",";
            os << "\n";
        }

        os << "]";
        return os;
    }

    template <typename T>
    T& AbstractField2D<T>::operator()(const size_t x, const size_t y)
    {
        return inner_data[x * y_size + y];
    }

    template <typename T>
    T AbstractField2D<T>::sample(const double x, const double y) const
    {
        const auto ix = static_cast<size_t>(std::floor(x));
        const auto iy = static_cast<size_t>(std::floor(y));

        const size_t ix1 = std::min(ix + 1, this->x_size - 1);
        const size_t iy1 = std::min(iy + 1, this->y_size - 1);

        const double fx = x - static_cast<double>(ix);
        const double fy = y - static_cast<double>(iy);

        T v00 = getValue(ix, iy);
        T v10 = getValue(ix1, iy);
        T v11 = getValue(ix1, iy1);
        T v01 = getValue(ix, iy1);

        return (1 - fx) * (1 - fy) * v00 +
            fx * (1 - fy) * v10 +
            fx * fy * v11 +
            (1 - fx) * fy * v01;
    }
}
#endif //ABSTRACTFIELD_H
