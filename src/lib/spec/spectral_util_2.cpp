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