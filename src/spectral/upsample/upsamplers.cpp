#include <upsample/upsamplers.h>
#include <upsample/glassner_naive.h>
#include <upsample/smits.h>
#include <upsample/sigpoly.h>

namespace spec {
    namespace upsamplers {
        const LazyPtr<IUpsampler> glassner{[]() -> auto { return new GlassnerUpsampler(); }};
        const LazyPtr<IUpsampler> smits{[]() -> auto { return new SmitsUpsampler(); }};
        const LazyPtr<IUpsampler> sigpoly{[]() -> auto { return new SigPolyUpsampler(); }};
    }
}