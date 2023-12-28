#include "math.h"

namespace spec {

    namespace math {

        Float clamp(Float x, Float a, Float b)
        {
            return x < a ? a : (x > b ? b : x);
        }

        vec3 clamp(const vec3 &x, Float a, Float b)
        {
            return {clamp(x.x, a, b), clamp(x.y, a, b), clamp(x.z, a, b)};
        }
        
    }

}