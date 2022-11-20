#include "Headers/SolveEquations.h"

namespace Equations
{

    Differential::Differential() = default;

    [[maybe_unused]] Differential::Differential(const arma::rowvec3 &M, const arma::rowvec3 &I, const arma::rowvec3& omega,
                               double gamma, const double psi, const double fi)
    : d_M(M), d_I(I), d_omega(omega), d_gamma(gamma), d_psi(psi), d_fi(fi) {}

    arma::vec6 Differential::solve()
    {
        arma::vec6 answer {};
        answer.zeros();

        double dt = 0.05, time = 0;

        while (time < time_end)
        {
            answer[0] += d_omega_x_dt() * dt;
            answer[1] += d_omega_y_dt() * dt;
            answer[2] += d_omega_z_dt() * dt;
            answer[3] +=  d_gamma_dt() * dt;
            answer[4] += d_psi_dt() * dt;
            answer[5] += d_fi_dt() * dt;
            time += dt;
        }
        return answer;
    }

    double Differential::d_omega_x_dt() {
        return (d_M[x] - (d_I[z] - d_I[y]) * d_omega[y] * d_omega[z]) / d_I[x];
    }
    double Differential::d_omega_y_dt() {
        return (d_M[y] - (d_I[x] - d_I[z]) * d_omega[x] * d_omega[z]) / d_I[y];
    }
    double Differential::d_omega_z_dt() {
        return (d_M[z] - (d_I[y] - d_I[x]) * d_omega[x] * d_omega[y]) / d_I[z];
    }
    double Differential::d_gamma_dt() {
        return d_omega[x] - tan(d_psi) * (d_omega[y] * cos(d_gamma) - d_omega[z]*sin(d_gamma));
    }
    double Differential::d_psi_dt() {
        return d_omega[z] * cos(d_gamma) - d_omega[y] * sin(d_gamma);
    }
    double Differential::d_fi_dt() {
        return (d_omega[y] * cos(d_gamma) - d_omega[z] * sin(d_gamma)) / cos(d_fi);
    }

    Differential::~Differential() = default;
}
