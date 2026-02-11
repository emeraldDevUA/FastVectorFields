//
// Created by GameRock on 31/01/2026.
//

#ifndef FASTVECTORFIELDS_ABSTRACTFIELD3D_H
#define FASTVECTORFIELDS_ABSTRACTFIELD3D_H

#include <vector>
#include <ostream>
#include <cmath>

#include <cereal/types/vector.hpp>

namespace vfFields
{
    template <typename T>

    class AbstractField3D
    {
    protected:
        std::vector<T> inner_data;
        size_t x_size;
        size_t y_size;
        size_t z_size;

    public:
        explicit AbstractField3D(const size_t grid_size)
            : inner_data(grid_size * grid_size * grid_size),
              x_size(grid_size),
              y_size(grid_size),
              z_size(grid_size)
        {
        }

        AbstractField3D(const size_t x_size, const size_t y_size, const size_t z_size)
            : inner_data(x_size * y_size * z_size),
              x_size(x_size),
              y_size(y_size),
              z_size(z_size)
        {
        }


        void setValue(const size_t x, const size_t y, const size_t z, const T value)
        {
            inner_data[x * y_size + y + (x_size * y_size) * z] = value;
        }

        T getValue(const size_t x, const size_t y, const size_t z) const
        {
            return inner_data[x * y_size + y + (x_size * y_size) * z];
        }

        [[nodiscard]] size_t getGridSizeX() const;

        [[nodiscard]] size_t getGridSizeY() const;

        [[nodiscard]] size_t getGridSizeZ() const;

        T operator()(double x, double y, double z) const;
        T& operator()(size_t x, size_t y, size_t z) const;

        bool operator==(const AbstractField3D& field) const
        {
            if (!(this->x_size == field.x_size &&
                this->y_size == field.y_size &&
                this->z_size == field.z_size))
            {
                return false;
            }
            const size_t full_size = this->x_size * this->y_size * this->z_size;

            for (size_t i = 0; i < full_size; ++i)
            {
                if (this->inner_data[i] != field.inner_data[i])
                {
                    return false;
                }
            }

            return true;
        }

        AbstractField3D operator+(const AbstractField3D& field) const;
        AbstractField3D operator-(const AbstractField3D& field) const;

        template <class Archive>
        void serialize(Archive& archive)
        {
            archive(
                cereal::make_nvp("x_size", this->x_size),
                cereal::make_nvp("y_size", this->y_size),
                cereal::make_nvp("z_size", this->z_size),
                cereal::make_nvp("inner_data", this->inner_data)
            );
        }
    };

    template <typename T>
    size_t AbstractField3D<T>::getGridSizeX() const
    {
        return x_size;
    }

    template <typename T>
    size_t AbstractField3D<T>::getGridSizeY() const
    {
        return y_size;
    }

    template <typename T>
    size_t AbstractField3D<T>::getGridSizeZ() const
    {
        return z_size;
    }


    template <typename T>
    std::ostream& operator<<(std::ostream& os, const AbstractField3D<T>& m)
    {
        for (size_t z = 0; z < m.getGridSizeZ(); ++z)
        {
            os << "z = " << z << '\n';

            for (size_t y = 0; y < m.getGridSizeY(); ++y)
            {
                for (size_t x = 0; x < m.getGridSizeX(); ++x)
                {
                    os << m.getValue(x, y, z) << ' ';
                }
                os << '\n';
            }

            os << '\n';
        }
        return os;
    }
    template <typename T>
    T& AbstractField3D<T>::operator()(const size_t x, const size_t y, const size_t z) const
    {
        return inner_data[x * y_size + y + (x_size * y_size) * z];
    };

    template <typename T>
    T AbstractField3D<T>::operator()(const double x, const double y, const double z) const
    {
        const auto ix = static_cast<size_t>(std::floor(x));
        const auto iy = static_cast<size_t>(std::floor(y));
        const auto iz = static_cast<size_t>(std::floor(z));

        const size_t ix1 = std::min(ix + 1, x_size - 1);
        const size_t iy1 = std::min(iy + 1, y_size - 1);
        const size_t iz1 = std::min(iz + 1, z_size - 1);

        const double fx = x - static_cast<double>(ix);
        const double fy = y - static_cast<double>(iy);
        const double fz = z - static_cast<double>(iz);

        // z = iz
        T v000 = getValue(ix, iy, iz);
        T v100 = getValue(ix1, iy, iz);
        T v010 = getValue(ix, iy1, iz);
        T v110 = getValue(ix1, iy1, iz);

        // z = iz1
        T v001 = getValue(ix, iy, iz1);
        T v101 = getValue(ix1, iy, iz1);
        T v011 = getValue(ix, iy1, iz1);
        T v111 = getValue(ix1, iy1, iz1);

        // interpolate along x
        T c00 = v000 * (1 - fx) + v100 * fx;
        T c10 = v010 * (1 - fx) + v110 * fx;
        T c01 = v001 * (1 - fx) + v101 * fx;
        T c11 = v011 * (1 - fx) + v111 * fx;

        // interpolate along y
        T c0 = c00 * (1 - fy) + c10 * fy;
        T c1 = c01 * (1 - fy) + c11 * fy;

        // interpolate along z
        return c0 * (1 - fz) + c1 * fz;
    }

    template <typename T>
    AbstractField3D<T> AbstractField3D<T>::operator+(const AbstractField3D& field) const
    {
        AbstractField3D newField(this->x_size, this->y_size, this->z_size);
        // Assuming both fields have the same dimensions
        for (size_t i = 0; i < this->x_size; i++)
        {
            T sum;
            for (size_t j = 0; j < this->y_size; ++j)
            {
                for (size_t k = 0; k < this->z_size; ++k)
                {
                    sum = this->getValue(i, j, k) + field.getValue(i, j, k);
                    newField.setValue(i, j, k, sum);
                }
            }
        }

        return newField;
    }


    template <typename T>
    AbstractField3D<T> AbstractField3D<T>::operator-(const AbstractField3D& field) const
    {
        AbstractField3D newField(this->x_size, this->y_size, this->z_size);
        // Assuming both fields have the same dimensions
        for (size_t i = 0; i < this->x_size; i++)
        {
            T difference;
            for (size_t j = 0; j < this->y_size; ++j)
            {
                for (size_t k = 0; k < this->z_size; ++k)
                {
                    difference = this->getValue(i, j, k) - field.getValue(i, j, k);
                    newField.setValue(i, j, k, difference);
                }
            }
        }

        return newField;
    }
}

#endif //FASTVECTORFIELDS_ABSTRACTFIELD3D_H
