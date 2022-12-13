#include "Headers/ChangeMetrics.h"

namespace MetricsLib
{
    [[maybe_unused]] arma::vec3 Metrics::transform_from_global_to_windy()
    {
        // fi-rotation
        arma::vec3 rotated_vec = rotate(g_vector, z_axis, g_fi);

        // gamma-rotation
        arma::vec3 rotated_axis_x = mul(rotate(z_axis, x_axis, g_fi), x_axis);
        rotated_vec = rotate(rotated_vec, rotated_axis_x, g_gamma);

        // psi-rotation
        arma::vec3 double_rotated_axis_x = mul(rotate(rotated_axis_x, z_axis, g_gamma), z_axis);
        rotated_vec = rotate(rotated_vec, double_rotated_axis_x, g_psi);

        return rotated_vec;
    }

    arma::vec3 Metrics::rotate(const arma::vec3& vec,
                                 const arma::vec3& axis,
                                 const double angle)
    {
        QuaternionLib::Quaternions q (cos(angle/2),
                                      arma::normalise(axis) * sin(angle/2));
        return q.rotation_matrix().i() * vec;
    }

    [[maybe_unused]] Metrics::Metrics(const arma::vec3 &vec, double gamma, double psi, double fi)
            : g_vector(vec), g_gamma(gamma), g_psi(psi), g_fi(fi) {}

    arma::vec3 mul(const arma::vec3 &a, const arma::vec3 &b) {
        return {
            a[y]*b[z] - a[z]*b[y],
            a[z]*b[x] - a[x]*b[z],
            a[x]*b[y] - a[y]*b[x]};
    }
}