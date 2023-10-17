#ifndef GLASSNER_SIMPLE_H
#define GLASSNER_SIMPLE_H
#include "upsampler.h"

class NaiveUpsampler : public IUpsampler
{
public:
	void upsample(const Image &sourceImage, SpectralImage &dest) const override;
	void upsample_pixel_to(const Pixel &pixel, Spectre &spectre) const override;
	~NaiveUpsampler() = default;
};

#endif