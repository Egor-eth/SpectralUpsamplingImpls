#ifndef UPSAMPLERS_H
#define UPSAMPLERS_H
#include <memory>
#include "upsampler.h"
#include "spec/basic_spectrum.h"
#include "spec/sigpoly_spectrum.h"


namespace spec {
    namespace upsamplers {
        extern const IUpsampler::shared_ptr glassner;
        extern const IUpsampler::shared_ptr smits;
    }
}
#endif