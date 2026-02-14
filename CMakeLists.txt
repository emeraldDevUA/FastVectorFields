cmake_minimum_required(VERSION 3.31)
project(FastVectorFields LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

# ---------------------------------------------------------
# 1. Shared source files (.cpp only)
# ---------------------------------------------------------
set(FAST_VECTOR_SOURCES

)

# ---------------------------------------------------------
# 2. Main executable
# ---------------------------------------------------------
add_executable(FastVectorFields
        tests/MainTest.cpp
        ${FAST_VECTOR_SOURCES}
)

target_include_directories(FastVectorFields PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/ExternalLibs/include
)

# ---------------------------------------------------------
# 3. Catch2
# ---------------------------------------------------------
include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.4.0
)

FetchContent_MakeAvailable(Catch2)

# ---------------------------------------------------------
# 4. Unit tests
# ---------------------------------------------------------
enable_testing()

add_executable(UnitTests
        tests/VectorTest.cpp
        tests/VectorFieldTest.cpp
        ${FAST_VECTOR_SOURCES}
)

target_include_directories(UnitTests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/ExternalLibs/include
)

target_link_libraries(UnitTests PRIVATE Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(UnitTests)

# ---------------------------------------------------------
# 5. Optimization
# ---------------------------------------------------------
if (MSVC)
    target_compile_options(FastVectorFields PRIVATE /O2)
    target_compile_options(UnitTests PRIVATE /O2)
else()
    target_compile_options(FastVectorFields PRIVATE -O3 -march=native -ffast-math)
    target_compile_options(UnitTests PRIVATE -O3 -march=native -ffast-math)
endif()
