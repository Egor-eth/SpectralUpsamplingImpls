#include <chrono>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <memory>
#include <unordered_map>
#include "upsamplers/glassner_naive.h"
#include "upsamplers/smits.h"
#include "imageutil/image.h"
#include "spec/spectral_image.h"
#include "spec/spectral_util.h"
#include "spec/conversions.h"
#include "common/format.h"
#include "argparse.h"

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;

using namespace spec;

std::unordered_map<std::string, std::unique_ptr<IUpsampler>> upsampler_by_method;

void fill_method_map() {
    upsampler_by_method["glassner"] = std::unique_ptr<IUpsampler>(new GlassnerUpsampler());
    upsampler_by_method["smits"] = std::unique_ptr<IUpsampler>(new SmitsUpsampler());
}

const IUpsampler *get_upsampler_for_method(const std::string &method_name)
{
    const auto it = upsampler_by_method.find(method_name);
    if(it == upsampler_by_method.end()) return nullptr;
    return it->second.get();
}

int downsample(const Args &args) {
    BasicSpectrum spectrum = spectral::load_spd(args.input_path);

    std::cout << "Converting spectum to RGB" << std::endl;
    vec3 downsampled_rgb = xyz2rgb(spectre2xyz(spectrum));
    std::cout << "Downsampled RGB: " << downsampled_rgb << std::endl;
    return 0;
}

int upsample(const Args &args) {
    const IUpsampler *upsampler = get_upsampler_for_method(*args.method);
    if(upsampler == nullptr) {
        std::cerr << "[!] Unknown method." << std::endl;
        return 1;
    }

    BasicSpectralImage spectral_img;
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
        upsampler->upsample(image, spectral_img);
        auto t2 = high_resolution_clock::now();
        std::cout << "Upsampling took " << duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms." << std::endl;
        std::cout << "Saving..." << std::endl;
        if(!spectral_img.save(*args.output_path)) {
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
