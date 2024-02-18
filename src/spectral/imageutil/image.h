#ifndef IMAGEUTIL_IMAGE_H
#define IMAGEUTIL_IMAGE_H
#include <string>
#include "pixel.h"
#include "base_image.h"

namespace spec {

    extern template class BaseImage<Pixel>;

    class Image : public BaseImage<Pixel>
    {
    public:
        using BaseImage<Pixel>::BaseImage;

        Image(const std::string &path);

        bool save(const std::string &path, const std::string &format = "") const;
    };

}

#endif 
