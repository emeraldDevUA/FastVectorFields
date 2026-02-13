cmake_minimum_required(VERSION 3.31)
project(FastVectorFields)

set(CMAKE_CXX_STANDARD 20)

# 1. Create a library for your logic (All files EXCEPT main.cpp)
# This allows both your app and your tests to use these classes.
add_library(FastVectorLib
        FieldBase/AbstractField2D.cpp
        FieldBase/AbstractField2D.hpp
        VectorFields/VectorField2D.hpp
        Vectors/Vector2D.hpp
        ScalarFields/ScalarField2D.hpp
        Vectors/Vector3D.hpp
        Interpolation/RBFInterpolator2D.hpp
        FieldBase/AbstractField3D.cpp
        FieldBase/AbstractField3D.hpp
        ScalarFields/ScalarField3D.hpp
        VectorFields/VectorField3D.hpp
        Interpolation/RBFInterpolator3D.hpp
        Interpolation/Solvers.hpp
        Vectors/Vector4D.hpp
)

# 2. Create your main application
# Only include main.cpp here. Link the library for the rest.
add_executable(FastVectorFields tests/MainTest.cpp
        Vectors/Vector3D.hpp
        Interpolation/RBFInterpolator2D.hpp
        FieldBase/AbstractField3D.cpp
        FieldBase/AbstractField3D.hpp
        ScalarFields/ScalarField3D.hpp
        VectorFields/VectorField3D.hpp
        Interpolation/RBFInterpolator3D.hpp
        Interpolation/Solvers.hpp
        Vectors/Vector4D.hpp
)

target_include_directories(FastVectorFields PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ExternalLibs/include)
target_include_directories(FastVectorLib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ExternalLibs/include)


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
        Vectors/Vector3D.hpp
        Interpolation/RBFInterpolator2D.hpp
        tests/VectorFieldTest.cpp
        FieldBase/AbstractField3D.cpp
        FieldBase/AbstractField3D.hpp
        ScalarFields/ScalarField3D.hpp
        VectorFields/VectorField3D.hpp
        Interpolation/RBFInterpolator3D.hpp
        Interpolation/Solvers.hpp
        Vectors/Vector4D.hpp)
target_link_libraries(UnitTests PRIVATE FastVectorLib Catch2::Catch2WithMain)
target_include_directories(UnitTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ExternalLibs/include)

if (MSVC)
    target_compile_options(UnitTests PRIVATE /O2)
    target_compile_options(FastVectorLib PRIVATE /O2)
else()
    target_compile_options(UnitTests PRIVATE -O3 -march=native)
    target_compile_options(FastVectorLib PRIVATE -O3 -march=native)
endif()

# 5. Integrate with CTest/IDE Test Explorers
include(Catch)
catch_discover_tests(UnitTests)