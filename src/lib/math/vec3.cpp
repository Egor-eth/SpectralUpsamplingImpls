#include "vec3.h" 
#include "ceres/jet.h"

namespace spec::math {

    template union base_vec3<Float>;

    template std::ostream &operator<<(std::ostream &str, const base_vec3<Float> &v);

}