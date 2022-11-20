#ifndef FLYINGSIMULATOR_SOLVEEQUATIONS_H
#define FLYINGSIMULATOR_SOLVEEQUATIONS_H

#include "armadillo"
#include <cmath>

namespace Equations
{
    enum PointType
    {
        x, y, z
    };

    class Differential
    {
    private:
        [[maybe_unused]] arma::rowvec3 d_V {}; // Speed Vector
        arma::rowvec3 d_M {}; // Resulting moment vector
        arma::rowvec3 d_I {}; // Inertia tensor vector

        arma::rowvec3 d_omega {}; // angular velocity vector
        double d_gamma = 0; // bank angle
        double d_psi = 0; // trim angle
        double d_fi = 0; // yaw angle

        double time_end = 1;

        double d_omega_x_dt();
        double d_omega_y_dt();
        double d_omega_z_dt();
        double d_gamma_dt();
        double d_psi_dt();
        double d_fi_dt();

    public:
        Differential();
        [[maybe_unused]] Differential(const arma::rowvec3& M, const arma::rowvec3& I, const arma::rowvec3& omega,
                     double gamma, double psi, double fi);

        ~Differential();

        arma::vec6 solve();


    };
}

#endif
