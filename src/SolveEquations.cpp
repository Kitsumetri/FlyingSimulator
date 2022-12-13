#include "Headers/SolveEquations.h"

namespace Equations
{
    Differential::Differential() = default;

    [[maybe_unused]] Differential::Differential
    (const arma::rowvec3 &M, const arma::rowvec3 &I, const arma::rowvec3& omega,
                               double gamma, const double psi, const double fi)
    : d_M(M), d_I(I), d_omega(omega), d_gamma(gamma), d_psi(psi), d_fi(fi) {}

    arma::vec6 Differential::solve()
    {
        double diff_omega_x = d_omega[x];
        double diff_omega_y = d_omega[y];
        double diff_omega_z = d_omega[z];

        double diff_gamma = d_gamma;
        double diff_psi = d_psi;
        double diff_fi = d_fi;
        double dt = 0.05, time = 0;
        while (time < time_end)
        {
            diff_omega_x += d_omega_x_dt(diff_omega_y, diff_omega_z) * dt;
            diff_omega_y += d_omega_y_dt(diff_omega_x, diff_omega_z) * dt;
            diff_omega_z += d_omega_z_dt(diff_omega_x, diff_omega_y) * dt;
            diff_gamma += d_gamma_dt(diff_gamma, diff_psi, diff_omega_x, diff_omega_y, diff_omega_z) * dt;
            diff_psi += d_psi_dt(diff_gamma, diff_omega_y, diff_omega_z) * dt;
            diff_fi += d_fi_dt(diff_gamma, diff_fi, diff_omega_y, diff_omega_z) * dt;

            get_output("/Users/nikolai/CLionProjects/FlyingSimulator/Output/graph_omega_x_dt.txt",
                       diff_omega_x, time);
            get_output("/Users/nikolai/CLionProjects/FlyingSimulator/Output/graph_omega_y_dt.txt",
                       diff_omega_y, time);
            get_output("/Users/nikolai/CLionProjects/FlyingSimulator/Output/graph_omega_z_dt.txt",
                       diff_omega_z, time);
            get_output("/Users/nikolai/CLionProjects/FlyingSimulator/Output/graph_gamma_dt.txt",
                       diff_gamma, time);
            get_output("/Users/nikolai/CLionProjects/FlyingSimulator/Output/graph_psi_dt.txt",
                       diff_psi, time);
            get_output("/Users/nikolai/CLionProjects/FlyingSimulator/Output/graph_fi_dt.txt",
                       diff_fi, time);
            time += dt;
        }
        return arma::vec6 {diff_omega_x, diff_omega_y, diff_omega_z, diff_gamma, diff_psi, diff_fi};
    }

    double Differential::d_omega_x_dt(const double diff_omega_y, const double diff_omega_z) {
        return (d_M[x] - (d_I[z] - d_I[y]) * diff_omega_y * diff_omega_z) / d_I[x];
    }
    double Differential::d_omega_y_dt(const double diff_omega_x, const double diff_omega_z) {
        return (d_M[y] - (d_I[x] - d_I[z]) * diff_omega_x * diff_omega_z) / d_I[y];
    }
    double Differential::d_omega_z_dt(const double diff_omega_x, const double diff_omega_y) {
        return (d_M[z] - (d_I[y] - d_I[x]) * diff_omega_x * diff_omega_y) / d_I[z];
    }
    double Differential::d_gamma_dt(const double diff_gamma, const double diff_psi,
                                    const double diff_omega_x, const double diff_omega_y, const double diff_omega_z) {
        return diff_omega_x - tan(diff_psi) * (diff_omega_y * cos(diff_gamma) - diff_omega_z*sin(diff_gamma));
    }
    double Differential::d_psi_dt(const double diff_gamma, const double diff_omega_y, const double diff_omega_z) {
        return diff_omega_z * cos(diff_gamma) - diff_omega_y * sin(diff_gamma);
    }
    double Differential::d_fi_dt(const double diff_gamma, const double diff_fi,
                                 const double diff_omega_y, const double diff_omega_z) {
        return (diff_omega_y * cos(diff_gamma) - diff_omega_z * sin(diff_gamma)) / cos(diff_fi);
    }

    void Differential::get_output(const std::string_view &file_name, double variable, double time) {
        std::fstream file;
        file.open(file_name, std::ios::out | std::ios::app);
        if (file.is_open())
        {
            file << time  << ' ' << variable << '\n';
            file.close();
        }
    }

    Differential::~Differential() = default;
}
