#include <utility>
#include <stdexcept>
#include <stb_image.h>
#include <stb_image_write.h>
#include "image.h"

Pixel *ImageLoader::operator()(const std::string &path, int &width, int &height) const
{
    (void) path;
    (void) width;
    (void) height;
    return nullptr;
}

void ImageLoader::free(Pixel *data) const
{
    (void) data;
}

int ImageSaver::operator()(const std::string &path, Pixel *data, int &width, int &height) const
{
    (void) data;
    (void) path;
    (void) width;
    (void) height;
    return 0;
}


template class BaseImage<Pixel, ImageLoader, ImageSaver>;
