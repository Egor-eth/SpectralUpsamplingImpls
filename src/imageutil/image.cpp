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
    if(ptr == nullptr) {
        stbi_failure_reason();
        throw std::runtime_error(std::format("Error reading image at {}", path));
    }
    return reinterpret_cast<Pixel *>(ptr); 
}

void ImageLoader::free(Pixel *data) const
{
    stbi_image_free(reinterpret_cast<unsigned char *>(data));
}

bool ImageSaver::operator()(const std::string &path, const Image &image, const std::string &format) const
{
    if(format == "png") {
        return stbi_write_png(path.c_str(), image.get_width(), image.get_height(), sizeof(Pixel), reinterpret_cast<const unsigned char *>(image.raw_data()), 0) != 0;
    }
    if(format == "jpg") {
        return stbi_write_jpg(path.c_str(), image.get_width(), image.get_height(), sizeof(Pixel), reinterpret_cast<const unsigned char *>(image.raw_data()), 90) != 0;
    }
    return false;
}


template class BaseImage<Pixel, ImageLoader, ImageSaver>;
