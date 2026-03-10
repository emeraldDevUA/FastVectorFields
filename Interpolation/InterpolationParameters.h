//
// Created by GameRock on 10/03/2026.
//

#ifndef FASTVECTORFIELDS_INTERPOLATIONPARAMETERS_H
#define FASTVECTORFIELDS_INTERPOLATIONPARAMETERS_H

namespace vfInterpolation
{
    enum class DistanceFunction
    {
        Euclidean,
        EuclideanSquared,
        Manhattan,
        Chebyshev,
        Minkowski
    };

    enum class RBFKernel
    {
        Gaussian,
        Multiquadric,
        InverseMultiquadric,
        ThinPlateSpline
    };
}


#endif //FASTVECTORFIELDS_INTERPOLATIONPARAMETERS_H