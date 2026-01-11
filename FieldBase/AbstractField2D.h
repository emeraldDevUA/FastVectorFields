//
// Created by GameRock on 06/01/2026.
//

#ifndef ABSTRACTFIELD_H
#define ABSTRACTFIELD_H
#include <iosfwd>
#include <vector>
#include <ostream>

template <typename T>

class AbstractField2D
{
protected:


public:
    std::vector<T> inner_data;

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

    const T* operator[](const size_t x) const
    {
        return &inner_data[x * y_size];
    }

    T* operator[](const size_t x)
    {
        return &inner_data[x * y_size];
    }

    AbstractField2D operator+(const AbstractField2D& field) const;
    AbstractField2D operator-(const AbstractField2D& field) const;


    // I will replace them with (double x, double y) version later
    //virtual T sample(double x, double y, double offset_x, double offset_y) const;
    //virtual T sample(double x, double y) const;
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const AbstractField2D<T>& m)
{
    for (size_t y = 0; y < m.y_size; ++y)
    {
        for (size_t x = 0; x < m.x_size; ++x)
        {
            os << m.inner_data[y * m.x_size + x] << ' ';
        }
        os << '\n';
    }
    return os;
}

#endif //ABSTRACTFIELD_H
