#include <upsample/functional/fourier.h>

namespace spec::upsample {
    
    FourierEmissionSpectrum fourier_emiss_int(const Pixel &pixel, Float power, const FourierLUT &lut)
    {
        std::vector<Float> coeffs = lut.eval(pixel.r, pixel.g, pixel.b, power);

        return FourierEmissionSpectrum{std::move(coeffs)};
    }

}