#ifndef UPSAMPLERS_SIGPOLY_H
#define UPSAMPLERS_SIGPOLY_H
#include "upsampler.h"
#include "spec/sp_lut.h"
#include "spec/sigpoly_spectrum.h"

namespace spec {
    class SigPolyUpsampler : public IUpsampler
    {
    public:
        SigPolyUpsampler();

        ISpectralImage::ptr upsample(const Image &sourceImage) const override;
        ISpectrum::ptr upsample_pixel(const Pixel &src) const override;
        ~SigPolyUpsampler() = default;
    private:
        const LUT luts[3];

        void upsample_pixel_to(const Pixel &pixel, SigPolySpectrum &s) const;
    };
}

#endif