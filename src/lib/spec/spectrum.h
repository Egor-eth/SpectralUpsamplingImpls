#ifndef SPEC_SPECTRUM_H
#define SPEC_SPECTRUM_H
#include "math/math.h"
#include <memory>
#include "common/refl.h"

namespace spec {
    class ISpectrum
    {
    public:
        INJECT_REFL(ISpectrum);

        virtual Float get_or_interpolate(Float w) const = 0;
        virtual ~ISpectrum() = default;

        static const ISpectrum &none();


        using ptr = std::unique_ptr<ISpectrum>;
    };

    class ISpectralImage {
    public:
        INJECT_REFL(ISpectralImage);
        
        ISpectralImage(int width, int height) 
            : width(width), height(height) {}

        virtual ISpectrum &at(int i, int j) = 0;
        virtual const ISpectrum &at(int i, int j) const = 0;

        inline int get_width() const { return width; }
        inline int get_height() const { return height; }

        virtual ~ISpectralImage() = default;

        using ptr = std::unique_ptr<ISpectralImage>;

    protected:
        int width, height;
    };

}

#endif