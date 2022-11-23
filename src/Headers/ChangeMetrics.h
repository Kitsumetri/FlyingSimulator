#ifndef FLYINGSIMULATOR_CHANGEMETRICS_H
#define FLYINGSIMULATOR_CHANGEMETRICS_H

#include "armadillo"
#include <cmath>

namespace Metrics
{
    arma::rowvec3 get_normal_metric(const arma::rowvec3& vec); // Нормальная(глобальная) система координат
    arma::rowvec3 get_speed_metric(const arma::rowvec3& vec); // Скоростная система координат
    arma::rowvec3 get_connected_metric(const arma::rowvec3& vec); // Связная система координат
}

#endif
