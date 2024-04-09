#ifndef INCLUDE_SPECTRAL_SPEC_FOURIER_SPECTRUM_H
#define INCLUDE_SPECTRAL_SPEC_FOURIER_SPECTRUM_H
#include <spectral/spec/spectrum.h>
#include <vector>

namespace spec {

    class FourierSpectrum : public ISpectrum 
    {
    public:
        INJECT_REFL(FourierSpectrum);

        FourierSpectrum()
            : coef{} {}

        FourierSpectrum(const FourierSpectrum &other) = default;

        FourierSpectrum(FourierSpectrum &&other)
            : coef(std::move(other.coef)) {}

        FourierSpectrum &operator=(const FourierSpectrum &other) = default;

        FourierSpectrum &operator=(FourierSpectrum &&other)
        {
            coef = std::move(other.coef);
        }

        Float &operator[](unsigned i)
        {
            return coef[i];
        }

        Float operator[](unsigned i) const
        {
            return coef[i];
        }

        const std::vector<Float> &get() const
        {
            return coef;
        }

        Float get_or_interpolate(Float w) const override;


    private:
        std::vector<Float> coef;
    };


    extern template class SpectralImage<FourierSpectrum>;

    using FourierSpectralImage = SpectralImage<FourierSpectrum>;

}
#endif