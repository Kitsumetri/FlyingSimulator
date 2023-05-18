#ifndef FLYINGSIMULATOR_SOLVEEQUATIONS_H
#define FLYINGSIMULATOR_SOLVEEQUATIONS_H

#include "armadillo"
#include <string_view>
#include <cmath>
#include <fstream>
#include <iostream>
#include <filesystem>

namespace Equations
{
    enum PointType
    {
        x,
        y,
        z
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

        double time_end = 7;

        static void get_output(const std::string_view& file_name, double variable, double time);

        double d_omega_x_dt(double diff_omega_y, double diff_omega_z);
        double d_omega_y_dt(double diff_omega_x, double diff_omega_z);
        double d_omega_z_dt(double diff_omega_x, double diff_omega_y);
        static double d_gamma_dt(double diff_gamma, double diff_psi,
                          double diff_omega_x, double diff_omega_y, double diff_omega_z);
        static double d_psi_dt(double diff_gamma, double diff_omega_y, double diff_omega_z);
        static double d_fi_dt(double diff_gamma, double diff_fi, double diff_omega_y, double diff_omega_z);

    public:
        Differential();
        [[maybe_unused]] Differential(const arma::rowvec3& M, const arma::rowvec3& I, const arma::rowvec3& omega,
                     double gamma, double psi, double fi);

        ~Differential();

        arma::vec6 solve();
    };
}
#endif