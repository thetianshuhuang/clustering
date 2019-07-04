/**
 *
 *
 */

#ifndef MISC_MATH_H
#define MISC_MATH_H

// log(Gamma_p(x)) -- log of multivariate gamma
double log_mv_gamma(int p, double x);

// Sample from weighted vector
int sample_weighted(double *weights, int length);


#endif