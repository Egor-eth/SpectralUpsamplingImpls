#include <upsample/functional/fourier.h>
#include <iostream>

namespace spec::upsample {
    
    FourierEmissionSpectrum fourier_emiss_int(const Pixel &pixel, Float power, const FourierLUT &lut)
    {
        vec3 rgb = pixel.to_vec3();
        Float max = rgb.max();
        if(max < 1.0f && max > 0.0f) {
            rgb = math::clamp(rgb / max, 0.0f, 1.0f);
            power *= max;
        }
        vec3i rgbi = (rgb * 255.0f).cast<int>();
        std::cout << "Evaluating " << rgbi << ", power " << power << std::endl;
        std::vector<Float> coeffs = lut.eval(rgbi.x, rgbi.y, rgbi.z, power);

        return FourierEmissionSpectrum{std::move(coeffs)};
    }

}