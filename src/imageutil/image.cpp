#include <utility>
#include <stdexcept>
#include <format>
#include <stb_image.h>
#include <stb_image_write.h>
#include "image.h"

Pixel *ImageLoader::operator()(const std::string &path, int &width, int &height) const
{
    int n;
    unsigned char *ptr = stbi_load(path.c_str(), &width, &height, &n, sizeof(Pixel));
    if(ptr == nullptr) throw std::runtime_error(std::format("Error reading image at %s", path));
    return reinterpret_cast<Pixel *>(ptr); 
}

void ImageLoader::free(Pixel *data) const
{
    stbi_image_free(reinterpret_cast<unsigned char *>(data));
}

bool ImageSaver::operator()(const std::string &path, const Pixel *data, int width, int height) const
{
    return stbi_write_png(path.c_str(), width, height, sizeof(Pixel), reinterpret_cast<const unsigned char *>(data), 0) != 0;
}


template class BaseImage<Pixel, ImageLoader, ImageSaver>;
