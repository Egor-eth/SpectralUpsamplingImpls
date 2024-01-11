#ifndef SPEC_SPECTRAL_IMAGE_H
#define SPEC_SPECTRAL_IMAGE_H
#include "spectrum.h"
#include <memory>

namespace spec {

   
    template <typename SpectrumType>
    class SpectralImage : public ISpectralImage
    {
    public:
        INJECT_REFL(SpectralImage<SpectrumType>);

        SpectralImage()
            : SpectralImage(nullptr, 0, 0) {}

        SpectralImage(int w, int h)
            : SpectralImage(new SpectrumType[w * h], w, h) {}

        SpectralImage(int w, int h, const SpectrumType &p)
            : SpectralImage(w, h)
        {
            std::fill(this->data.get(), this->data.get() + w * h, p);
        }    

        SpectralImage(const SpectralImage &image)
            : SpectralImage(new SpectrumType[width * height], image.width, image.height)
        {
            std::copy(image.data.get(), image.data.get() + width * height, data.get());
        }

        SpectralImage(SpectralImage &&image)
            : ISpectralImage(image.width, image.height), data(std::move(image.data)) {}

        inline const SpectrumType *raw_data() const {
            return data.get();
        }

        inline SpectrumType *raw_data() {
            return data.get();
        }

        SpectrumType &at(int i, int j) override
        {
            long pos = (i + j * width);
            if(pos < 0 || pos >= width * height) throw std::out_of_range("Requested pixel is out of range");
            return data[pos];
        }

        const SpectrumType &at(int i, int j) const override
        {
            long pos = (i + j * width);
            if(pos < 0 || pos >= width * height) throw std::out_of_range("Requested pixel is out of range");
            return data[pos];
        }

        SpectralImage &operator=(SpectralImage &&other) {
            if(this == &other) return *this;

            std::swap(data, other.data);
            width = std::move(other.width);
            height = std::move(other.height);
            return *this;
        }



    protected:
        std::unique_ptr<SpectrumType []> data;

        SpectralImage(SpectrumType *ptr, int width, int height)
            : ISpectralImage(width, height), data(ptr) {}


    };

}


#endif 
