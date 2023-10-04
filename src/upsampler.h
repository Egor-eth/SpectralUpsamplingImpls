#ifndef COMMON_H
#define COMMON_H
#include "imageutil/spectral_image.h"
#include "imageutil/image.h"

class Upsampler
{
public:
	virtual void upsample(const Image &sourceImage, SpectralImage &dest) const = 0;

	virtual ~Upsampler() {}
};


#endif