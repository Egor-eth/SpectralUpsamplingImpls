#include <chrono>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <memory>
#include <unordered_map>
#include "upsamplers/upsamplers.h"
#include "imageutil/image.h"
#include "spec/basic_spectrum.h"
#include "spec/spectral_util.h"
#include "spec/conversions.h"
#include "common/format.h"
#include "argparse.h"

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;

using namespace spec;

std::unordered_map<std::string, IUpsampler::shared_ptr> upsampler_by_name;

void fill_method_map() {
    upsampler_by_name["glassner"] = spec::upsamplers::glassner;
    upsampler_by_name["smits"] = spec::upsamplers::smits;
    upsampler_by_name["sigpoly"] = spec::upsamplers::sigpoly;
}

const IUpsampler *get_upsampler_by_name(const std::string &method_name)
{
    const auto it = upsampler_by_name.find(method_name);
    if(it == upsampler_by_name.end()) return nullptr;
    return it->second.get();
}

int downsample(const Args &args) {
    BasicSpectrum spectrum = spec::util::load_spd(args.input_path);

    std::cout << "Converting spectum to RGB" << std::endl;
    vec3 downsampled_rgb = xyz2rgb(spectre2xyz(spectrum));
    std::cout << "Downsampled RGB: " << downsampled_rgb << std::endl;
    return 0;
}

int upsample(const Args &args) {
    const IUpsampler *upsampler = get_upsampler_by_name(*args.method);
    if(upsampler == nullptr) {
        std::cerr << "[!] Unknown method." << std::endl;
        return 1;
    }

    Image image;
    if(args.color) {
        image = Image(1, 1);
        image.at(0, 0) = *args.color;
    }
    else {
        image = Image(args.input_path);
    }

    try {
        std::cout << "Converting image to spectral with " << *args.method << " method..." << std::endl;
        auto t1 = high_resolution_clock::now();
        ISpectralImage::ptr spectral_img = upsampler->upsample(image);
        auto t2 = high_resolution_clock::now();
        std::cout << "Upsampling took " << duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms." << std::endl;
        std::cout << "Saving..." << std::endl;

        vec3 downsampled_rgb = xyz2rgb(spectre2xyz(spectral_img->at(0, 0)));
        std::cout << "Downsampled RGB: " << downsampled_rgb << std::endl;

        if(!spec::util::save(args.output_dir, *args.output_name, *spectral_img)) {
            std::cerr << "[!] Error saving image." << std::endl;
        }
    } catch (std::exception &ex) {
        std::cerr << "[!] " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}

/**
 *  -c (hex):    use color code instead of texture
 *  -f (path):   path to texture
 *  -m (method): method to use
 */
int main(int argc, char **argv)
{
    fill_method_map();

    Args args;
    if(!parse_args(argc, argv, args)) return 1;

    return args.downsample_mode ? downsample(args) : upsample(args);
} 
