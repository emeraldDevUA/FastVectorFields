//
// Created by GameRock on 07/01/2026.
//

#ifndef NUMERICALCONSTANTS_H
#define NUMERICALCONSTANTS_H

#include <limits>

constexpr float FLOAT_EPSILON = std::numeric_limits<float>::min();
constexpr double DOUBLE_EPSILON = std::numeric_limits<double>::min();

template <typename T>
inline T get_epsilon()
{
    return std::numeric_limits<T>::min();
}

#endif //NUMERICALCONSTANTS_H
