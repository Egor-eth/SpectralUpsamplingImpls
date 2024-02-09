#ifndef SPEC_SIGPOLY_SPECTRUM_H
#define SPEC_SIGPOLY_SPECTRUM_H
#include <stdexcept>
#include "spectral_image.h"

namespace spec {
    
    class SigPolySpectrum : public ISpectrum
    {
    public:

        INJECT_REFL(SigPolySpectrum);

        SigPolySpectrum() noexcept(true)
            : ISpectrum(), coef{0, 0, 0} {}

        SigPolySpectrum(const SigPolySpectrum &s) = default;

        SigPolySpectrum(const vec3 &c) noexcept(true)
            : ISpectrum(), coef{c.x, c.y, c.z} {}
        
        Float &operator[](int i) noexcept(true)
        {
            return coef[i];
        }

        Float operator[](int i) const noexcept(true)
        {
            return coef[i];
        }

        Float get_or_interpolate(Float w) const override;

        SigPolySpectrum &operator=(const SigPolySpectrum &other) = default;

    private:
        Float coef[3];
    };

    extern template class SpectralImage<SigPolySpectrum>;

    using SigPolySpectralImage = SpectralImage<SigPolySpectrum>;
    
}

#endif
