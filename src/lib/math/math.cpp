#include "math.h"

namespace spec {

    namespace math {

        namespace {

            Float sigmoid(Float x)
            {
                return std::fma(0.5, x / std::sqrt(std::fma(x, x, 1)), 0.5);
            }
            
        }

        Float clamp(Float x, Float a, Float b)
        {
            return x < a ? a : (x > b ? b : x);
        }

        vec3 clamp(const vec3 &x, Float a, Float b)
        {
            return {clamp(x.x, a, b), clamp(x.y, a, b), clamp(x.z, a, b)};
        }
        
        Float sigmoid_polynomial(Float x, const Float coef[3])
        {
            return sigmoid(std::fma(std::fma(coef[0], x, coef[1]), x, coef[2]));
        }
        
    }

}