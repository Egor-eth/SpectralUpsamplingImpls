#ifndef UPSAMPLERS_SMITS_H
#define UPSAMPLERS_SMITS_H
#include "upsampler.h"

class SmitsUpsampler : public IUpsampler
{
    void upsample(const Image &sourceImage, SpectralImage &dest) const override;
    void upsample_pixel_to(const Pixel &pixel, Spectrum &spectre) const override;
    ~SmitsUpsampler() = default;
};

#endif