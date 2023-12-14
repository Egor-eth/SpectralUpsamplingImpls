#ifndef UPSAMPLERS_GLASSNER_NAIVE_H
#define UPSAMPLERS_GLASSNER_NAIVE_H
#include "upsampler.h"

class GlassnerUpsampler : public IUpsampler
{
public:
    void upsample(const Image &sourceImage, SpectralImage &dest) const override;
    void upsample_pixel_to(const Pixel &pixel, Spectrum &spectre) const override;
    ~GlassnerUpsampler() = default;
};

#endif