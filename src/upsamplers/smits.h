#ifndef UPSAMPLERS_SMITS_H
#define UPSAMPLERS_SMITS_H
#include "upsampler.h"

namespace spec {
    class SmitsUpsampler : public IUpsampler
    {
        void upsample(const Image &sourceImage, BasicSpectralImage &dest) const override;
        void upsample_pixel_to(const Pixel &pixel, BasicSpectrum &spectre) const override;
        ~SmitsUpsampler() = default;
    };
}
#endif