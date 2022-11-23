#ifndef FLYINGSIMULATOR_QUATERNIONLIB_H
#define FLYINGSIMULATOR_QUATERNIONLIB_H

#include "armadillo"
#include <cmath>
#include <iostream>

namespace QuaternionLib {
    class [[maybe_unused]] Quaternions
    {
    private:
        const char variables[4] = {' ', 'i', 'j', 'k'};
        double q_w {0}; double q_x {0}; double q_y {0}; double q_z {0};
        arma::rowvec4 q_Q {q_w, q_x, q_y, q_z};

        enum PointType
        {
            w, // 0
            x, // 1
            y, // 2
            z  // 3
        };

        void update_vector();
    public:
        // Constructors
        Quaternions();
        [[maybe_unused]] explicit Quaternions(const arma::rowvec4& vec);
        Quaternions(double w, double x, double y, double z);
        Quaternions(const Quaternions &other_Q);

        // Destructor
        ~Quaternions();

        // Methods
        [[maybe_unused]] [[maybe_unused]] void print_q_vec() const;

        [[maybe_unused]] [[nodiscard]] double get_w() const;
        [[maybe_unused]] [[nodiscard]] double get_x() const;
        [[maybe_unused]] [[nodiscard]] double get_y() const;
        [[maybe_unused]] [[nodiscard]] double get_z() const;

        [[maybe_unused]] void set_w(double w);
        [[maybe_unused]] void set_x(double x);
        [[maybe_unused]] void set_y(double y);
        [[maybe_unused]] void set_z(double z);

        [[maybe_unused]] void zeros();
        [[maybe_unused]] [[nodiscard]] double magnitude() const;
        [[maybe_unused]]  [[nodiscard]] Quaternions normalize() const;
        [[maybe_unused]] [[nodiscard]] Quaternions conjugate() const;
        [[maybe_unused]] [[nodiscard]] Quaternions inverse() const;
        [[maybe_unused]] [[nodiscard]] arma::mat rotation_matrix() const;
        [[maybe_unused]] [[nodiscard]] Quaternions absolute() const;

        // Operators
        Quaternions operator+(double alpha) const;
        Quaternions operator+(const Quaternions& other_q) const;
        Quaternions operator-(double alpha) const;
        Quaternions operator-(const Quaternions& other_q) const;
        Quaternions operator*(double alpha) const;
        Quaternions operator*(const Quaternions& other_q) const;
        Quaternions operator/(double alpha) const;
        Quaternions operator/(const Quaternions& other_q) const;
    };
}
#endif