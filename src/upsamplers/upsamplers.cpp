#include "upsamplers.h"
#include "glassner_naive.h"
#include "smits.h"

namespace spec {
    namespace upsamplers {
        const IUpsampler::shared_ptr glassner{new GlassnerUpsampler()};
        const IUpsampler::shared_ptr smits{new SmitsUpsampler()};
    }
}