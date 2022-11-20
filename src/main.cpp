#include "Headers/SolveEquations.h"

int main()
{
    arma::rowvec3 I {1, 2, 3};
    arma::rowvec3 omega {0.3, 10, 11};
    arma::rowvec3 M {2, 3, 2};
    Equations::Differential diff {M, I, omega, 0.4, 0.4,0.6};

    auto ans = diff.solve();
    ans.print();
    return 0;
}