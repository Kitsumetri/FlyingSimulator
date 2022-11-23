#include "Headers/QuaternionLib.h"

namespace QuaternionLib
{
    // Constructor
    Quaternions::Quaternions() = default;
    [[maybe_unused]] Quaternions::Quaternions(const arma::rowvec4 &vec)
    : q_w(vec[w]), q_x(vec[x]), q_y(vec[y]), q_z(vec[z]), q_Q(vec) {}
    Quaternions::Quaternions(const Quaternions &other_Q) = default;
    Quaternions::Quaternions(double w_q, double x_q, double y_q, double z_q)
    : q_w(w_q), q_x(x_q), q_y(y_q), q_z(z_q), q_Q({w_q, x_q, y_q, z_q}) {}

    // Destructor
    Quaternions::~Quaternions() = default;

    // Methods
    [[maybe_unused]] void Quaternions::print_q_vec() const
    {
        for (int index = 0; index < q_Q.size(); ++index)
        {
            switch (index)
            {
                case 0:
                    std::cout << q_Q[index] << " + ";
                    break;
                case 3:
                    std::cout << q_Q[index] << '*' << variables[index] << std::endl;
                    break;
                default:
                    std::cout << q_Q[index] << '*' << variables[index] << " + ";
            }
        }
    }

    [[maybe_unused]] double Quaternions::get_w() const { return q_w; }
    [[maybe_unused]] double Quaternions::get_x() const { return q_x; }
    [[maybe_unused]] double Quaternions::get_y() const { return q_y; }
    [[maybe_unused]] double Quaternions::get_z() const { return q_z; }

    [[maybe_unused]] void Quaternions::set_w(double w) {
        q_w = w;
        update_vector();
    }
    [[maybe_unused]] void Quaternions::set_x(double x) {
        q_x = x;
        update_vector();
    }
    [[maybe_unused]] void Quaternions::set_y(double y) {
        q_y = y;
        update_vector();
    }
    [[maybe_unused]] void Quaternions::set_z(double z) {
        q_z = z;
        update_vector();
    }

    void Quaternions::update_vector() { q_Q = {q_w, q_x, q_y, q_z}; }

    [[maybe_unused]] void Quaternions::zeros() {
        q_x = q_y = q_z = q_w = 0;
        q_Q.zeros();
    }
    [[maybe_unused]] Quaternions Quaternions::absolute() const {
        return {abs(q_w), abs(q_x), abs(q_y), abs(q_z)};
    }
    [[maybe_unused]] double Quaternions::magnitude() const {
        return sqrt(q_w*q_w + q_x*q_x + q_y*q_y + q_z*q_z);
    }
    [[maybe_unused]] Quaternions Quaternions::normalize() const {
        return  *this/magnitude();
    }
    [[maybe_unused]] Quaternions Quaternions::conjugate() const {
        return {q_w, -q_x, -q_y, -q_z};
    }
    [[maybe_unused]] Quaternions Quaternions::inverse() const {
        return conjugate()/(pow(magnitude(), 2));
    }
    [[maybe_unused]] arma::mat Quaternions::rotation_matrix() const
    {
        arma::mat33 Q_mat;
        Q_mat = {
                {1 - 2*q_y*q_y - 2*q_z*q_z, 2*q_x*q_y - 2*q_z*q_w, 2*q_x*q_z - 2*q_y*q_w},
                {2*q_x*q_y + 2*q_z*q_w, 1 - 2*q_x*q_x - 2*q_z*q_z, 2*q_y*q_z - 2*q_x*q_w},
                {2*q_x*q_z - 2*q_y*q_w, 2*q_y*q_z + 2*q_x*q_w, 1 - 2*q_x*q_x - 2*q_y*q_y}
        };
        return Q_mat;
    }

    // Operators
    Quaternions Quaternions::operator+(const double alpha) const {
        return {q_w + alpha, q_x, q_y, q_z};
    }
    Quaternions Quaternions::operator+(const Quaternions& other_q) const {
        return {q_w + other_q.q_w, q_x + other_q.q_x, q_y + other_q.q_y, q_z + other_q.q_z};
    }
    Quaternions Quaternions::operator-(const double alpha) const {
        return {q_w - alpha, q_x, q_y, q_z};
    }
    Quaternions Quaternions::operator-(const Quaternions& other_q) const {
        return {q_w - other_q.q_w, q_x - other_q.q_x, q_y - other_q.q_y, q_z - other_q.q_z};
    }
    Quaternions Quaternions::operator*(const double alpha) const {
        return {q_w * alpha, q_x * alpha, q_y * alpha, q_z * alpha};
    }
    Quaternions Quaternions::operator*(const Quaternions &other_q) const {
        return {
            q_w*other_q.q_w - q_x*other_q.q_x - q_y*other_q.q_y - q_z*other_q.q_z,
            q_w*other_q.q_x + q_x*other_q.q_w + q_y*other_q.q_z - q_z*other_q.q_y,
            q_w*other_q.q_y - q_x*other_q.q_z + q_y*other_q.q_w + q_z*other_q.q_x,
            q_w*other_q.q_z + q_x*other_q.q_y - q_y*other_q.q_x + q_z*other_q.q_w
        };
    }
    Quaternions Quaternions::operator/(const double alpha) const {
        return {q_w/alpha, q_x/alpha, q_y/alpha, q_z/alpha};
    }
    Quaternions Quaternions::operator/(const Quaternions &other_q) const
    {
        double d = pow(other_q.q_w, 2) + pow(other_q.q_x, 2) +
                pow(other_q.q_y, 2) + pow(other_q.q_z, 2);
        return {
            (q_w*other_q.q_w + q_x*other_q.q_x + q_y*other_q.q_y + q_z*other_q.q_z)/d,
            (q_x*other_q.q_w - q_w*other_q.q_x - q_z*other_q.q_y + q_y*other_q.q_z)/d,
            (q_y*other_q.q_w + q_z*other_q.q_x - q_w*other_q.q_y - q_x*other_q.q_z)/d,
            (q_z*other_q.q_w - q_y*other_q.q_x - q_x*other_q.q_y - q_w*other_q.q_z)/d
        };
    }
}