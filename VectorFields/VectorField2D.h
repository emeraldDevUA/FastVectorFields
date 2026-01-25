//
// Created by GameRock on 07/01/2026.
//

#ifndef VECTORFIELD2D_H
#define VECTORFIELD2D_H

#include "../FieldBase/AbstractField2D.h"
#include "../ScalarFields/ScalarField2D.h"
#include  "../Vectors/Vector3D.h"
#include  "../Interpolation/RBFInterpolator2D.h"


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
        T threshold = 1e-6;

        std::vector<Vector3D<T>> refCos;
        std::vector<Vector3D<T>> refSin;

        for (size_t i = 0; i < this->getGridSizeX(); ++i)
        {
            for (size_t j = 0; j < this->getGridSizeY(); ++j)
            {
                auto v = this->getValue(i, j);
                if (v.length() > threshold)
                {
                    refCos.push_back({(T)i, v.x, (T)j});
                    refSin.push_back({(T)i, v.y, (T)j});
                }
            }
        }

        if (refCos.size() < 3) return;

        T epsilon = 0.8;

        RBFInterpolator2D<T> rbfCos(refCos, epsilon);
        RBFInterpolator2D<T> rbfSin(refSin, epsilon);

        for (size_t i = 0; i < this->getGridSizeX(); ++i)
        {
            for (size_t j = 0; j < this->getGridSizeY(); ++j)
            {
                T cosVal = rbfCos.evaluate((T)i, (T)j);
                T sinVal = rbfSin.evaluate((T)i, (T)j);

                T len = std::sqrt(cosVal * cosVal + sinVal * sinVal);
                if (len > threshold)
                {
                    cosVal /= len;
                    sinVal /= len;
                }

                this->setValue(i, j, Vector2D<T>(cosVal, sinVal));
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

    VectorField2D operator+(const VectorField2D& field) const
    {
        VectorField2D newField(this->x_size, this->y_size);
        // Assuming both fields have the same dimensions
        for (size_t i = 0; i < this->x_size; i++)
        {
            for (size_t j = 0; j < this->y_size; ++j)
            {
                newField.setValue(i, j, this->getValue(i, j) + field.getValue(i, j));
            }
        }

        return newField;
    }

    VectorField2D<T> operator-(const VectorField2D& field) const
    {
        VectorField2D newField(this->x_size, this->y_size);
        // Assuming both fields have the same dimensions
        for (size_t i = 0; i < this->x_size; i++)
        {
            Vector2D<T> difference;
            for (size_t j = 0; j < this->y_size; ++j)
            {
                difference = this->getValue(i, j) - field.getValue(i, j);
                newField.setValue(i, j, difference);
            }
        }

        return newField;
    }
};


#endif //VECTORFIELD2D_H
