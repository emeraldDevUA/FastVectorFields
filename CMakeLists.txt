cmake_minimum_required(VERSION 3.31)
project(FastVectorFields)

set(CMAKE_CXX_STANDARD 20)

add_executable(FastVectorFields main.cpp
        FieldBase/AbstractField2D.cpp
        FieldBase/AbstractField2D.h
        VectorFields/VectorField2D.cpp
        VectorFields/VectorField2D.h
        Vectors/Vector2D.cpp
        Vectors/Vector2D.h
        NumericalConstants.h
        ScalarFields/ScalarField2D.cpp
        ScalarFields/ScalarField2D.h
)
