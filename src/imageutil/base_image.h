#ifndef IMAGEUTIL_BASE_IMAGE_H
#define IMAGEUTIL_BASE_IMAGE_H
#include <string>
#include <stdexcept>
#include <algorithm>

template<typename T>
constexpr bool canSave = false;
template<typename T>
constexpr bool canLoad = false;

template<typename T> 
concept Saveable = canSave<T>;

template<typename T> 
concept Loadable = canLoad<T>;

template <typename Pixel>
class BaseImage
{
public:

    BaseImage(int w, int h)
        : data(), width(w), height(h), path(), loaded(false)
    {
        data = new unsigned char[w * h * sizeof(Pixel)];
    }    

    BaseImage(int w, int h, const Pixel &p)
        : BaseImage(w, h)
    {
        Pixel *ptr = reinterpret_cast<Pixel*>(data);
        std::fill(ptr, ptr + w * h * sizeof(Pixel), p);
    }    


    BaseImage(const std::string &path) requires Loadable<BaseImage>;

    BaseImage(BaseImage &&image)
        : data(), width(std::move(image.width)), height(std::move(image.height)), path(std::move(image.path)), loaded(std::move(image.loaded))
    {
        data = image.data;
        image.data = nullptr;
    }

    ~BaseImage()
    {
        delete[] data;
    }

    Pixel &at(int i, int j)
    {
        long pos = (i + j * width);
        if(pos < 0 || pos >= width * height) throw std::out_of_range("Requested pixel is out of range");
        return reinterpret_cast<Pixel*>(data)[pos];
    }

    int save(const std::string &p) const requires Saveable<BaseImage>;
    int save() const requires Saveable<BaseImage>;


    int get_width() const { return width; }
    int get_height() const { return height; }

private:
    unsigned char *data;
    int width, height;
    std::string path;
    bool loaded;

};


#endif 
