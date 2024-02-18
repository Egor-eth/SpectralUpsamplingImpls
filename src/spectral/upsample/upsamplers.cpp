#include "upsamplers.h"
#include "glassner_naive.h"
#include "smits.h"
#include "sigpoly.h"

namespace spec {
    namespace upsamplers {
        const LazyPtr<IUpsampler> glassner{[]() -> auto { return new GlassnerUpsampler(); }};
        const LazyPtr<IUpsampler> smits{[]() -> auto { return new SmitsUpsampler(); }};
        const LazyPtr<IUpsampler> sigpoly{[]() -> auto { return new SigPolyUpsampler(); }};
    }
}