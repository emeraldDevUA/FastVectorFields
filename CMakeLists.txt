cmake_minimum_required(VERSION 3.31)
project(FastVectorFields)

set(CMAKE_CXX_STANDARD 20)

# 1. Create a library for your logic (All files EXCEPT main.cpp)
# This allows both your app and your tests to use these classes.
add_library(FastVectorLib
        FieldBase/AbstractField2D.cpp
        FieldBase/AbstractField2D.h
        VectorFields/VectorField2D.cpp
        VectorFields/VectorField2D.h
        Vectors/Vector2D.cpp
        Vectors/Vector2D.h
        ScalarFields/ScalarField2D.cpp
        ScalarFields/ScalarField2D.h
        NumericalConstants.h
        Vectors/Vector3D.cpp
        Vectors/Vector3D.h
        Interpolation/RBFInterpolator2D.cpp
        Interpolation/RBFInterpolator2D.h
)

# 2. Create your main application
# Only include main.cpp here. Link the library for the rest.
add_executable(FastVectorFields tests/MainTest.cpp
        Vectors/Vector3D.cpp
        Vectors/Vector3D.h
        Interpolation/RBFInterpolator2D.cpp
        Interpolation/RBFInterpolator2D.h
)
target_link_libraries(FastVectorFields PRIVATE FastVectorLib)

# ---------------------------------------------------------
# 3. Add Catch2 Testing Framework
# ---------------------------------------------------------
include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.4.0
)

FetchContent_MakeAvailable(Catch2)

# 4. Set up Testing
enable_testing()

# Create the test executable (Ensure tests/test_main.cpp exists!)
add_executable(UnitTests tests/VectorTest.cpp
        Vectors/Vector3D.cpp
        Vectors/Vector3D.h
        Interpolation/RBFInterpolator2D.cpp
        Interpolation/RBFInterpolator2D.h
        tests/VectorFieldTest.cpp)
target_link_libraries(UnitTests PRIVATE FastVectorLib Catch2::Catch2WithMain)

# 5. Integrate with CTest/IDE Test Explorers
include(Catch)
catch_discover_tests(UnitTests)