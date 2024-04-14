#include <internal/math/fourier.h>
#include <internal/math/levinson.h>
#include <cassert>

#include <iostream>

namespace spec::math {

    Float to_phase(Float wl, Float start, Float end)
    {
        return std::fma(PI, (wl - start) / (end - start), -PI);
    }

    std::vector<Float> real_fourier_moments_of(const std::vector<Float> &phases, const std::vector<Float> &values, int n)
    {
        const unsigned N = phases.size();
        assert(N == values.size());
        int M = n - 1;
        std::vector<Float> moments(M + 1);
        for(int i = 0; i <= M; ++i) {
            Complex val{0.0f, 0.0f};
            for(unsigned j = 0; j < N; ++j) {
                val += values[j] * std::exp(-I * Float(i) * phases[j]);
            }
            moments[i] = std::real(val) / Float(N);
        }
        return moments;
    }

    std::vector<Complex> fourier_moments_of(const std::vector<Float> &phases, const std::vector<Float> &values, int n)
    {
        const unsigned N = phases.size();
        assert(N == values.size());
        int M = n - 1;
        std::vector<Complex> moments(M + 1);
        for(int i = 0; i <= M; ++i) {
            Complex val{0.0f, 0.0f};
            for(unsigned j = 0; j < N; ++j) {
                val += values[j] * std::exp(-I * Float(i) * phases[j]);
            }
            moments[i] = val / Float(N);
        }
        return moments;
    }

    namespace {

        std::vector<Complex> exponential_moments(const std::vector<Float> moments, Complex &gamma0)
        {
            gamma0 = 0.5f * INV_TWO_PI * std::exp(PI * I * (moments[0] - 0.5f));
            const int M = moments.size() - 1;
            std::vector<Complex> res(M + 1);
            res[0] = 2.0f * gamma0.real();
            for(int i = 1; i <= M; ++i) {
                Complex sm{0.0f, 0.0f};
                for(int j = 1; j <= i - 1; ++j) {
                    sm += Float(i - j) * res[j] * moments[i - j]; 
                }

                res[i] = TWO_PI * I * (Float(i) * gamma0 * moments[i] + sm) / Float(i);
            }
            return res;
        }

    }

    std::vector<Complex> lagrange_multipliers(const std::vector<Float> &moments)
    {      
        const int M = moments.size() - 1;
        Complex gamma0;
        std::vector<Complex> gamma = exponential_moments(moments, gamma0);

        //Calculating q
        std::vector<Float> e0(M + 1, 0.0f);
        e0[0] = 1.0f;
        std::vector<Complex> data(2 * M + 1); //Matrix values
        data[M] = INV_TWO_PI * gamma[0];
        for(int i = 1; i <= M; ++i) {
            data[M + i] = INV_TWO_PI * gamma[i];
            data[M - i] = INV_TWO_PI * std::conj(gamma[i]);
        }
        std::vector<Complex> q = levinson<Complex>(data, e0);

      /*  std::cout << "q: ";
        for(const auto &v : q) {
            std::cout << v << " ";
        }
        std::cout << std::endl;
       */

        std::vector<Complex> lambda(M + 1);
        for(int i = 0; i <= M; ++i) {
            Complex t{0.0f, 0.0f};

            for(int k = 0; k <= M - i; ++k) {
                Complex ts{0.0f, 0.0f};
                for(int j = 0; j <= M - k - i; ++j) {
                    ts += std::conj(q[j + k + i]) * q[j];
                }
                t += ts * (k == 0 ? gamma0 : gamma[k]);
            }
            lambda[i] = t / (PI * I * q[0]);
        }
        return lambda;
    }
    
    Float bounded_mese_l(Float phase, const std::vector<Complex> &lagrange_m)
    {
        const int M = lagrange_m.size() - 1;
        Float sum = std::real(lagrange_m[0]);
        for(int i = 1; i <= M; ++i) {
            sum += 2.0f * std::real(lagrange_m[i] * std::exp(-I * Float(i) * phase));
        } 

        return INV_PI * std::atan(sum) + 0.5f;
    }

    std::vector<Float> mese(std::vector<Float> phases, const std::vector<Float> &gamma)
    {
        int M = gamma.size() - 1;
        std::vector<Float> e0(M + 1, 0.0f);
        e0[0] = 1.0f;

        std::vector<Complex> data(2 * M + 1); //Matrix values
        data[M] = INV_TWO_PI * gamma[0];
        for(int i = 1; i <= M; ++i) {
            data[M + i] = INV_TWO_PI * gamma[i];
            data[M - i] = INV_TWO_PI * gamma[i]; //[std::conj(gamma[i])] in fact;
        }

        std::vector<Complex> q = levinson<Complex>(data, e0);

        for(unsigned k = 0; k < phases.size(); ++k) {
         //   for(int i = 0; i <= M; ++i) y[i] = INV_TWO_PI * std::exp(-I * Float(i) * phases[k]);
            Complex t = 0.0f;
            for(int i = 0; i <= M; ++i) t += INV_TWO_PI * std::conj(q[i]) * std::exp(-I * Float(i) * phases[k]); 


            Float div = std::fabs(t);
            div *= div;

            phases[k] = INV_TWO_PI * std::real(q[0]) / div;
        }
        return phases;
    }

}