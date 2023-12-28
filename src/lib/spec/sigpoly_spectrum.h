#ifndef SPEC_SIGPOLY_SPECTRUM_H
#define SPEC_SIGPOLY_SPECTRUM_H

#include "spectrum.h"
#include <stdexcept>

namespace spec {
    
    class SigPolySpectrum : public ISpectrum
    {
    public:
        SigPolySpectrum()
            : coef{0, 0, 0} {}

        SigPolySpectrum(const SigPolySpectrum &s) = default;
        
        inline Float &operator[](int i)
        {
            return coef[i];
        }

        inline Float operator[](int i) const
        {
            return coef[i];
        }

        Float get_or_interpolate(Float w) const override;

        SigPolySpectrum &operator=(const SigPolySpectrum &other) = default;

    private:
        Float coef[3];
    };

}

#endif
