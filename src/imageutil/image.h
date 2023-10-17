#ifndef IMAGEUTIL_IMAGE_H
#define IMAGEUTIL_IMAGE_H
#include <string>
#include "pixel.h"
#include "base_image.h"

class ImageLoader
{
public:
    Pixel *operator()(const std::string &path, int &width, int &height) const;
    void free(Pixel *data) const;
};


class ImageSaver
{
public:
    bool operator()(const std::string &path, const BaseImage<Pixel, ImageLoader, ImageSaver> &image, const std::string &format = "") const;
};

using Image = BaseImage<Pixel, ImageLoader, ImageSaver>;

#endif 
