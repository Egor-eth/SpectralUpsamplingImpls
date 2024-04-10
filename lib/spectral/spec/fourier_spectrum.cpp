#include <spec/fourier_spectrum.h>

namespace spec {

    Float MFourierSpectrum::get_or_interpolate(Float w) const
    {
        if(lagrange_m.empty()) {
            lagrange_m = math::lagrange_multipliers(coef);
        }

        return math::bounded_mese_l(math::to_phase(w), lagrange_m);
    }

    Float LFourierSpectrum::get_or_interpolate(Float w) const
    {
        return math::bounded_mese_l(math::to_phase(w), coef);  
    }

    template class SpectralImage<MFourierSpectrum>;
    template class SpectralImage<LFourierSpectrum>;

}