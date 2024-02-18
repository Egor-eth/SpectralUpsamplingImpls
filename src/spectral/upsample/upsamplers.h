#ifndef UPSAMPLE_UPSAMPLERS_H
#define UPSAMPLE_UPSAMPLERS_H
#include "upsampler.h"
#include "spec/basic_spectrum.h"
#include "spec/sigpoly_spectrum.h"
#include "common/lazy_value.h"


namespace spec {
    namespace upsamplers {
        extern const LazyPtr<IUpsampler> glassner;
        extern const LazyPtr<IUpsampler> smits;
        extern const LazyPtr<IUpsampler> sigpoly;
    }
}
#endif