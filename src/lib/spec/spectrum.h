#ifndef SPEC_SPECTRUM_H
#define SPEC_SPECTRUM_H
#include "math/math.h"

namespace spec {
    class ISpectrum
    {
    public: 
        virtual Float get_or_interpolate(Float w) const = 0;
        virtual ~ISpectrum() = default;

        static const ISpectrum &none();
    };

}

#endif