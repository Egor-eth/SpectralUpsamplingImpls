#include <internal/math/fourier.h>
#include <cassert>

namespace spec::math {

    Float to_phase(Float wl, Float start, Float end)
    {
        return std::fma(PI, (wl - start) / (end - start), -PI);
    }

    std::vector<Float> fourier_moments_of(const std::vector<Float> &phases, const std::vector<Float> &values, int m)
    {
        const unsigned N = phases.size();
        assert(N == values.size());
        std::vector<Float> moments(m + 1);
        for(int i = 0; i <= m; ++i) {
            Complex val{0.0f, 0.0f};
            for(unsigned j = 0; j < N; ++j) {
                val += INV_TWO_PI * values[j] * std::exp(-I * Float(i) * phases[j]);
            }
            moments[i] = std::real(val);
        }
        return moments;
    }

    namespace {

        std::vector<Complex> levinson(const std::vector<Complex> &gamma, const std::vector<Float> &y)
        {
            (void) gamma;
            (void) y;
            return {}; //TODO
        }

        std::vector<Complex> exponential_moments(const std::vector<Float> moments, Complex &gamma0)
        {
            gamma0 = 0.5f * INV_TWO_PI * std::exp(PI * I * (moments[0] - 0.5f));
            const int M = moments.size() - 1;
            std::vector<Complex> res(M + 1);
            res[0] = 2 * gamma0.real();
            for(int i = 1; i <= M; ++i) {
                Complex sm{0.0f, 0.0f};
                for(int j = 1; j <= i - 1; ++j) {
                    sm += Float(i - j) * res[j] * moments[i - j]; 
                }

                res[i] = TWO_PI * I / Float(i) * (Float(i) * gamma0 * moments[i] + sm);
            }
            return res;
        }

    }

    std::vector<Complex> lagrange_multipliers(const std::vector<Float> &moments)
    {      
        const int M = moments.size() - 1;
        Complex gamma0;
        std::vector<Complex> gamma = exponential_moments(moments, gamma0);

        std::vector<Float> e0(M + 1);
        e0[0] = 1.0f;
        std::vector<Complex> q = levinson(gamma, e0);
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

}