#include <upsample/functional/fourier.h>

namespace spec::upsample {
    
    FourierEmissionSpectrum fourier_int(const Pixel &pixel, Float power, const FourierLUT &lut)
    {
        std::vector<Float> coeffs = lut.eval(pixel.r, pixel.g, pixel.b);
        for(Float &c : coeffs) c *= power;

        return FourierEmissionSpectrum{std::move(coeffs)};
    }

}