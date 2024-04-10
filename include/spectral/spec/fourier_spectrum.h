#ifndef INCLUDE_SPECTRAL_SPEC_FOURIER_SPECTRUM_H
#define INCLUDE_SPECTRAL_SPEC_FOURIER_SPECTRUM_H
#include <spectral/spec/spectrum.h>
#include <spectral/spec/spectral_image.h>
#include <spectral/internal/math/fourier.h>
#include <vector>

namespace spec {

    class MFourierSpectrum : public ISpectrum 
    {
    public:
        INJECT_REFL(MFourierSpectrum);

        MFourierSpectrum()
            : coef{} {}

        MFourierSpectrum(const std::vector<Float> &coef)
            : coef(coef) {}

        MFourierSpectrum(std::vector<Float> &&coef)
            : coef(std::move(coef)) {}

        MFourierSpectrum(const MFourierSpectrum &other) = default;

        MFourierSpectrum(MFourierSpectrum &&other)
            : coef(std::move(other.coef)) {}

        MFourierSpectrum &operator=(const MFourierSpectrum &other) = default;

        MFourierSpectrum &operator=(MFourierSpectrum &&other)
        {
            coef = std::move(other.coef);
            return *this;
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

        void set(const std::vector<Float> &c)
        {
            coef = c;
        }

        Float get_or_interpolate(Float w) const override;


    private:
        std::vector<Float> coef;
        mutable std::vector<Complex> lagrange_m{};
    };


    extern template class SpectralImage<MFourierSpectrum>;

    using MFourierSpectralImage = SpectralImage<MFourierSpectrum>;


    class LFourierSpectrum : public ISpectrum 
    {
    public:
        INJECT_REFL(LFourierSpectrum);

        LFourierSpectrum()
            : coef{} {}

        LFourierSpectrum(const std::vector<Complex> &coef)
            : coef(coef) {}

        LFourierSpectrum(std::vector<Complex> &&coef)
            : coef(std::move(coef)) {}

        LFourierSpectrum(const LFourierSpectrum &other) = default;

        LFourierSpectrum(LFourierSpectrum &&other)
            : coef(std::move(other.coef)) {}

        LFourierSpectrum &operator=(const LFourierSpectrum &other) = default;

        LFourierSpectrum &operator=(LFourierSpectrum &&other)
        {
            coef = std::move(other.coef);
            return *this;
        }

        Complex &operator[](unsigned i)
        {
            return coef[i];
        }

        Complex operator[](unsigned i) const
        {
            return coef[i];
        }

        const std::vector<Complex> &get() const
        {
            return coef;
        }

        void set(const std::vector<Complex> &c)
        {
            coef = c;
        }

        Float get_or_interpolate(Float w) const override;


    private:
        std::vector<Complex> coef;
    };


    extern template class SpectralImage<LFourierSpectrum>;

    using LFourierSpectralImage = SpectralImage<LFourierSpectrum>;

}
#endif