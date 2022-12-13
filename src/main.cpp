#include "Headers/QuaternionLib.h"
#include "Headers/SolveEquations.h"
#include "cmath"

int main()
{
    arma::rowvec3 I {5, 3.1, 10};
    arma::rowvec3 omega {5.4, -3, 6};
    arma::rowvec3 M {2, 3, 2};
    Equations::Differential diff {M, I, omega, M_PI/3, M_PI/3,M_PI/6};

    auto ans = diff.solve();
    ans.print();
    return 0;
}