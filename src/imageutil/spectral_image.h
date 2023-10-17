#ifndef IMAGEUTIL_SPECTRAL_IMAGE_H
#define IMAGEUTIL_SPECTRAL_IMAGE_H
#include <memory>
#include <unordered_set>
#include "constants.h"
#include "base_image.h"
#include "spectre.h"


class SpectralSaver
{
public:
    bool operator()(const std::string &path, const Spectre *data, int width, int height) const;
};

class SpectralImage : public BaseImage<Spectre, void, SpectralSaver>
{
public:
    SpectralImage()
        : BaseImage<Spectre, void, SpectralSaver>() {}
    SpectralImage(int w, int h)
        : BaseImage<Spectre, void, SpectralSaver>(w, h) {}  

    SpectralImage(int w, int h, const Spectre &p)
        : BaseImage<Spectre, void, SpectralSaver>(w, h, p) {} 

    SpectralImage(const SpectralImage &img)
        : BaseImage<Spectre, void, SpectralSaver>(img) {}

    SpectralImage(SpectralImage &&image)
        : BaseImage<Spectre, void, SpectralSaver>(std::move(image)) {}

    SpectralImage &operator=(SpectralImage &&other)
    {
        BaseImage<Spectre, void, SpectralSaver>::operator=(std::move(other));
        return *this;
    }

private:
    std::shared_ptr<std::unordered_set<SpectreFloat>> wavelenghts;
};


#endif 
