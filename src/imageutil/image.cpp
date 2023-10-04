#include <utility>
#include <stdexcept>
#include <stb_image.h>
#include <stb_image_write.h>

#include "image.h"

template<>
constexpr bool canSave<BaseImage<PixelRGBA>> = true;
template<>
constexpr bool canLoad<BaseImage<PixelRGBA>> = true;


template<>
BaseImage<PixelRGBA>::BaseImage(const std::string &p)
    : data(), width(), height(), path(p), loaded(true)
{   
    int n;
    data = stbi_load(path.c_str(), &width, &height, &n, sizeof(Pixel));
    if(data == nullptr) throw std::runtime_error("Could not load image");
}

template<>
BaseImage<PixelRGBA>::~BaseImage()
{
    if(loaded) {
        stbi_image_free(data);
    } else {
        delete[] data;
    }
}

template<>
int BaseImage<PixelRGBA>::save(const std::string &p) const
{
    int i = stbi_write_png(p.c_str(), width, height, sizeof(Pixel), data, 0);
    return i;
}

template<>
int BaseImage<PixelRGBA>::save() const
{
    return save(path);
}

template class BaseImage<PixelRGBA>;