#include "sigpoly_spectrum.h"
#include "math/math.h"

namespace spec {

    Float SigPolySpectrum::get_or_interpolate(Float w) const
    {
        return math::sigmoid_polynomial(w, coef);
    }
    
    template class SpectralImage<SigPolySpectrum>;

}