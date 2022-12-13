#ifndef FLYINGSIMULATOR_CHANGEMETRICS_H
#define FLYINGSIMULATOR_CHANGEMETRICS_H

#include "armadillo"
#include <cmath>
#include "QuaternionLib.h"
#include "SolveEquations.h"

using Equations::x, Equations::y, Equations::z;

namespace MetricsLib
{
    arma::vec3 mul(const arma::vec3& a, const arma::vec3& b);

    class Metrics
    {
    private:
        arma::vec3 g_vector = {};
        double g_gamma = 0, g_psi = 0, g_fi = 0;

        const arma::vec3 x_axis = {1, 0, 0};
        const arma::vec3 z_axis = {0, 0, 1};

        static arma::vec3 rotate(const arma::vec3& vec, const arma::vec3& axis, double angle);

    public:
        [[maybe_unused]] Metrics(const arma::vec3& vec, double gamma, double psi, double fi);

        [[maybe_unused]] arma::vec3 transform_from_global_to_windy();
    };
}

#endif
