#include "spectral_util.h"
#include <fstream>
#include <stdexcept>
#include "math/math.h"
#include "common/csv.h"
#include <vector>
#include <tuple>

using namespace spec;

namespace spec::util
{

    const LazyValue<BasicSpectrum> CIE_D6500{[]() -> auto { return util::load_spd("resources/cie.stdillum.D6500.spd"); }};

    Float get_cie_y_integral()
    {
        static Float val = 0.0f;
        static bool not_computed = true;
        if(not_computed) {
            for(int lambda = CURVES_WAVELENGHTS_START; lambda <= CURVES_WAVELENGHTS_END; lambda += CURVES_WAVELENGHTS_STEP) {
            //for(int lambda : wl) {
                Float lightval = CIE_D6500->get_or_interpolate(lambda);
                val += _interp<Y_CURVE>(lambda) * CURVES_WAVELENGHTS_STEP * lightval;
            }
            not_computed = false;
        }
        return val;
    }

    BasicSpectrum load_spd(const std::string &path)
    {
        std::ifstream file(path);
        if(!file) throw std::runtime_error("Cannot open file");

        BasicSpectrum sp;

        std::vector<std::tuple<Float, Float>> loaded = csv::load_as_vector<Float, Float>(file, ' ');
        for(const auto &p : loaded) {
            sp.set(std::get<0>(p), std::get<1>(p));
        }
        return sp;
    }

}