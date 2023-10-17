#ifndef IMAGEUTIL_BASE_IMAGE_H
#define IMAGEUTIL_BASE_IMAGE_H
#include <string>
#include <stdexcept>
#include <algorithm>

/*namespace img {
    template<typename T>
    constexpr bool can_save = false;
    template<typename T>
    constexpr bool can_load = false;

    template<typename T>
    concept Saveable = can_save<T>;
    template<typename T>
    concept Loadable = can_load<T>;
}*/

template <typename PixelType, typename Loader = void, typename Saver = void>
class BaseImage
{
public:

    BaseImage()
        : data(), width(0), height(0), load_path(), loaded(false) {}

    BaseImage(int w, int h)
        : data(), width(w), height(h), load_path(), loaded(false)
    {
        data = new PixelType[w * h];
    }    

    BaseImage(int w, int h, const PixelType &p)
        : BaseImage(w, h)
    {
        std::fill(data, data + w * h, p);
    }    

    template<typename T = Loader, typename = std::enable_if_t<!std::is_same<T, void>::value>>
    explicit BaseImage(const std::string &path)
        : load_path(path), loaded(true)
    {
        data = Loader{}(load_path, width, height);
        if(data == nullptr) throw std::runtime_error("Could not load images");
    }

    BaseImage(const BaseImage &image)
        : data(), width(image.width), height(image.height), load_path(image.load_path), loaded(image.loaded)
    {
        data = new PixelType[width * height];
        std::copy(image.data, image.data + width * height, data);
    }

    BaseImage(BaseImage &&image)
        : data(), width(std::move(image.width)), height(std::move(image.height)), load_path(std::move(image.load_path)), loaded(std::move(image.loaded))
    {
        data = image.data;
        image.data = nullptr;
    }

    ~BaseImage()
    {   
        if constexpr(!std::is_same<Loader, void>::value) {
            if(loaded) {
                Loader{}.free(data);
            } else {
                delete[] data;
            }
        } else {
            delete[] data;
        }
    }

    PixelType &at(int i, int j)
    {
        long pos = (i + j * width);
        if(pos < 0 || pos >= width * height) throw std::out_of_range("Requested pixel is out of range");
        return reinterpret_cast<PixelType*>(data)[pos];
    }

    const PixelType &at(int i, int j) const
    {
        long pos = (i + j * width);
        if(pos < 0 || pos >= width * height) throw std::out_of_range("Requested pixel is out of range");
        return reinterpret_cast<PixelType*>(data)[pos];
    }

    template<typename T = Saver, typename = std::enable_if_t<!std::is_same<T, void>::value>>
    bool save(const std::string &path) const
    {
        return Saver{}(path, data, width, height);
    }

    template<typename T = Loader, typename P = Saver, typename = std::enable_if_t<!std::is_same<P, void>::value && !std::is_same<T, void>::value, int>>
    bool save() const
    {
        if(!loaded) throw std::runtime_error("Image was not loaded, unknown path.");
        return save(load_path);
    }

    BaseImage &operator=(BaseImage &&other) {
        if(this == &other) return *this;

        std::swap(data, other.data);
        width = std::move(other.width);
        height = std::move(other.height);
        load_path = std::move(other.load_path);
        loaded = other.loaded;
        return *this;
    }

    int get_width() const { return width; }
    int get_height() const { return height; }

private:
    PixelType *data;
    int width, height;
    std::string load_path;
    bool loaded;

};


#endif 
