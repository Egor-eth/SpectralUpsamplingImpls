#ifndef IMAGEUTIL_SPECTRAL_IMAGE_H
#define IMAGEUTIL_SPECTRAL_IMAGE_H
#include "constants.h"
#include "base_image.h"
#include "spectre.h"


class SpectralImage : public BaseImage<Spectre, void, void>
{
public:
    SpectralImage(int w, int h)
        : BaseImage(w, h) {}  

    SpectralImage(int w, int h, const Spectre &p)
        : BaseImage(w, h, p) {} 


    SpectralImage(SpectralImage &&image)
        : BaseImage(std::move(image)) {}

private:

};


#endif 
