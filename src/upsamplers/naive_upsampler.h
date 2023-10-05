#ifndef GLASSNER_SIMPLE_H
#define GLASSNER_SIMPLE_H
#include "upsampler.h"

class NaiveUpsampler : public Upsampler
{
public:
	void upsample(const Image &sourceImage, SpectralImage &dest) const override;

	~NaiveUpsampler() = default;
};

#endif