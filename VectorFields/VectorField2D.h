//
// Created by GameRock on 07/01/2026.
//

#ifndef VECTORFIELD2D_H
#define VECTORFIELD2D_H

#include "../FieldBase/AbstractField2D.h"
#include "../ScalarFields/ScalarField2D.h"
#include  "../Vectors/Vector3D.h"
#include "../Interpolation/Interpolation.h"


using vfMath::Vector3D;

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
        : AbstractField2D<Vector2D<T>>(field.getGridSizeX(), field.getGridSizeY())
    {
        for (size_t i = 1; i + 1 < field.getGridSizeX(); ++i)
        {
            for (size_t j = 1; j + 1 < field.getGridSizeY(); ++j)
            {
                this->setValue(i, j, field.gradient(i, j, 1));
            }
        }
    }

    T divergence(size_t i, size_t j) const
    {
        const T eps = 1e-6;

        // Assuming your class provides access to u(i,j) and v(i,j)
        // You might need to adapt these if your storage is different

        //assume that u(i, j) is this(i, j).x and v(i, j) is this(i, j).y

        T dudx = (this->getValue(i + 1, j).x - this->getValue(i - 1, j).x) / (2.0 * eps);
        T dvdy = (this->getValue(i, j + 1).y - this->getValue(i, j - 1).y) / (2.0 * eps);

        return dudx + dvdy;
    }


    T curl(size_t i, size_t j, T eps = 10e-6) const
    {
        // For 2D vector field (u, v), curl = dv/dx - du/dy
        T dvdx = (this->getValue(i + 1, j).y - this->getValue(i - 1, j).y) / (2.0 * eps);
        T dudy = (this->getValue(i, j + 1).x - this->getValue(i, j - 1).x) / (2.0 * eps);

        // Return as Vector2D with curl value in z-component represented as x, y=0
        // Or if you want the scalar curl, consider changing return type to T
        return dvdx - dudy;
    }


    void fillWithInterpolation()
    {
        T epsilon = 10e-6;
        std::vector<Vector3D<T>> referencePoints{};

        Vector2D<T> xAxis(1.0, 0);
        for (size_t i = 0; i < this->getGridSizeX(); ++i)
        {
            for (size_t j = 0; j < this->getGridSizeY(); ++j)
            {
                if (this->getValue(i, j).length() > epsilon)
                {
                    T value = this->getValue(i, j).dot(xAxis);
                    referencePoints.push_back(Vector3D<T>(i, value, j));
                }
            }
        }

        for (size_t i = 0; i < this->getGridSizeX(); ++i)
        {
            for (size_t j = 0; j < this->getGridSizeY(); ++j)
            {
                Vector2D<T> P0(static_cast<T>(i), static_cast<T>(j));
                T interpolatedValue = Interpolation<T>().lagrangeInterpolation3D(P0, referencePoints);
                this->setValue(i, j, vfMath::Vector2D<T>(interpolatedValue, interpolatedValue));
            }
        }
    }

    void normalize()
    {
        for (size_t i = 1; i + 1 < this->getGridSizeX(); ++i)
        {
            for (size_t j = 1; j + 1 < this->getGridSizeY(); ++j)
            {
                if (this->getValue(i, j).length() > 10e-6)
                    this->getValue(i, j).normalize();
            }
        }
    }
};


#endif //VECTORFIELD2D_H
