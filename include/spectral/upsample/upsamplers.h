#ifndef INCLUDE_SPECTRAL_UPSAMPLE_UPSAMPLERS_H
#define INCLUDE_SPECTRAL_UPSAMPLE_UPSAMPLERS_H
#include <spectral/upsample/upsampler.h>
#include <spectral/spec/basic_spectrum.h>
#include <spectral/spec/sigpoly_spectrum.h>
#include <spectral/internal/common/lazy_value.h>


namespace spec {
    namespace upsamplers {
        extern const LazyPtr<IUpsampler> glassner;
        extern const LazyPtr<IUpsampler> smits;
        extern const LazyPtr<IUpsampler> sigpoly;
    }
}
#endif