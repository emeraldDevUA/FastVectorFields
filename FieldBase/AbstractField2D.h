//
// Created by GameRock on 06/01/2026.
//

#ifndef ABSTRACTFIELD_H
#define ABSTRACTFIELD_H
#include <iosfwd>
#include <vector>
#include <ostream>
#include <cmath>

template <typename T>

class AbstractField2D
{
protected:
    std::vector<T> inner_data;

public:
    size_t x_size;
    size_t y_size;

    explicit AbstractField2D(const size_t grid_size)
        : x_size(grid_size),
          y_size(grid_size),
          inner_data(grid_size * grid_size)
    {
    }

    AbstractField2D(size_t x_size, const size_t y_size)
        : x_size(x_size),
          y_size(y_size),
          inner_data(x_size * y_size)
    {
    }


    void setValue(const size_t x, const size_t y, const T value)
    {
        inner_data[x * y_size + y] = value;
    }


    T getValue(const size_t x, const size_t y)
    {
        return inner_data[x * y_size + y];
    }


    T getValue(const size_t x, const size_t y) const
    {
        return inner_data[x * y_size + y];
    }

    size_t getGridSizeX() const;

    size_t getGridSizeY() const;

    T operator()(double x, double y) const;


    AbstractField2D operator+(const AbstractField2D& field) const;
    AbstractField2D operator-(const AbstractField2D& field) const;


    // I will replace them with (double x, double y) version later
    //virtual T sample(double x, double y, double offset_x, double offset_y) const;
    //virtual T sample(double x, double y) const;
};

template <typename T>
size_t AbstractField2D<T>::getGridSizeX() const
{
    return x_size;
}

template <typename T>
size_t AbstractField2D<T>::getGridSizeY() const
{
    return y_size;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const AbstractField2D<T>& m)
{
    for (size_t y = 0; y < m.getGridSizeY(); ++y)
    {
        for (size_t x = 0; x < m.getGridSizeX(); ++x)
        {
            os << m.getValue(x, y) << ' ';
        }
        os << '\n';
    }
    return os;
}


template <typename T>

T AbstractField2D<T>::operator()(double x, double y) const
{
    const auto ix = static_cast<size_t>(std::floor(x));
    const auto iy = static_cast<size_t>(std::floor(y));

    size_t ix1 = std::min(ix + 1, this->x_size - 1);
    size_t iy1 = std::min(iy + 1, this->y_size - 1);

    const double fx = x - ix;
    const double fy = y - iy;

    T v00 = getValue(ix, iy);
    T v10 = getValue(ix1, iy);
    T v11 = getValue(ix1, iy1);
    T v01 = getValue(ix, iy1);

    return (1 - fx) * (1 - fy) * v00 +
        fx * (1 - fy) * v10 +
        fx * fy * v11 +
        (1 - fx) * fy * v01;
}
#endif //ABSTRACTFIELD_H
