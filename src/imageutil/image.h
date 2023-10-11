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
    int operator()(const std::string &path, Pixel *data, int &width, int &height) const;
};

using Image = BaseImage<Pixel, ImageLoader, ImageSaver>;

#endif 
