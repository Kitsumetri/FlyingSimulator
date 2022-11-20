#ifndef FLYINGSIMULATOR_SOLVEEQUATIONS_H
#define FLYINGSIMULATOR_SOLVEEQUATIONS_H

#include "armadillo"

namespace Equations
{
    class Differential
    {
    private:
        arma::rowvec3 V {}; // Speed Vector
        arma::rowvec3 M {}; // Resulting moment vector
        arma::mat33 I {}; // Inertia tensor matrix

        arma::rowvec3 omega {}; // angular velocity vector
        double gamma; // bank angle
        double psi; // trim angle
        double fi; // yaw angle
    };
}

#endif
