#include "spectral_util.h"
#include <fstream>
#include <stdexcept>
#include "math/math.h"
#include "serialization/csv.h"
#include "serialization/binary.h"
#include <vector>
#include <tuple>
#include <unordered_map>
#include <filesystem>

using namespace spec;
namespace fs = std::filesystem;

namespace spec::util
{

    const LazyValue<BasicSpectrum> CIE_D6500{[]() -> auto { return load_spd("resources/cie.stdillum.D6500.spd"); }};

    Float get_cie_y_integral()
    {
        static LazyValue<Float> value{[]() -> Float { 
            Float val;
            for(int lambda = CURVES_WAVELENGHTS_START; lambda <= CURVES_WAVELENGHTS_END; lambda += CURVES_WAVELENGHTS_STEP) {
            //for(int lambda : wl) {
                Float lightval = CIE_D6500->get_or_interpolate(lambda);
                val += _interp<Y_CURVE>(lambda) * CURVES_WAVELENGHTS_STEP * lightval;
            }
            return val;
        }};
       return *value;
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

    BasicSpectralImage load_png1(const std::string &meta_path)
    {
        return {};
    }

    SigPolySpectrum load_sigpoly(const std::string &path)
    {
        std::ifstream file(path);
        if(!file) throw std::runtime_error("Cannot open file");

        std::tuple<Float, Float, Float> loaded = csv::parse_line<Float, Float, Float>(file, ' ');

        return {{std::get<0>(loaded), std::get<1>(loaded), std::get<2>(loaded)}};
    }

    SigPolySpectralImage load_sigpoly_img(const std::string &path)
    {
        std::ifstream file{path};
        if(!file) throw std::runtime_error("Cannot open file");

        const uint64_t marker = binary::read<uint64_t>(file);
        if(marker != SIGPOLY_FILE_MARKER) throw std::runtime_error("Incorrect file format");
        const uint64_t floatsize = binary::read<uint16_t>(file);
        if(floatsize != sizeof(Float)) throw std::runtime_error("Unsupported float size in image");
        const uint32_t width = binary::read<uint32_t>(file);
        const uint32_t height = binary::read<uint32_t>(file);

        SigPolySpectralImage img{width, height};
        auto *ptr = img.raw_data();

        for(uint32_t i = 0; i < width * height; ++i) {
            ptr[i].set(binary::read_vec<Float>(file));
        }

        return img;
    }

    namespace {
        enum class SpectrumFormat 
        {
            UNKNOWN = 0,
            SIGPOLY,
            BASIC_SPD
        };

        SpectrumFormat _guess_spec_format(const std::string &ext)
        {
            static const std::unordered_map<std::string, SpectrumFormat> formats{
                {"spd", SpectrumFormat::BASIC_SPD},
                {"spspec", SpectrumFormat::SIGPOLY}
            };

            auto it = formats.find(ext);
            if(it != formats.end()) {
                return it->second;
            }
            return SpectrumFormat::UNKNOWN;
        }

        enum class SpectralImgFormat 
        {   
            UNKNOWN = 0,
            SIGPOLY_SIF,
            BASIC_PNG1_JSON
        };

        SpectralImgFormat _guess_specimg_format(const std::string &ext)
        {
            static const std::unordered_map<std::string, SpectralImgFormat> formats{
                {"sif", SpectralImgFormat::SIGPOLY_SIF},
                {"json", SpectralImgFormat::BASIC_PNG1_JSON}
            };

            auto it = formats.find(ext);
            if(it != formats.end()) {
                return it->second;
            }
            return SpectralImgFormat::UNKNOWN;
        }

    }



    bool load(const std::string path, ISpectrum::ptr &s)
    {
        fs::path p{path};
        SpectrumFormat f = _guess_spec_format(p.extension());
        switch(f) {
        case SpectrumFormat::SIGPOLY:
            s.reset(new SigPolySpectrum(load_sigpoly(path)));
            return true;
        case SpectrumFormat::BASIC_SPD:
            s.reset(new BasicSpectrum(load_spd(path)));
            return true;
        default:
            return false;
        }
    }

    bool load(const std::string path, ISpectralImage::ptr &img)
    {
        fs::path p{path};
        SpectralImgFormat f = _guess_specimg_format(p.extension());
        try {
            switch(f) {
            case SpectralImgFormat::SIGPOLY_SIF:
                img.reset(new SigPolySpectralImage(load_sigpoly_img(path)));
                return true;
            case SpectralImgFormat::BASIC_PNG1_JSON:
                img.reset(new BasicSpectralImage(load_png1(path)));
                return true;
            default:
                return false;
            }
        }
        catch(...) {
            return false;
        }
    }

}