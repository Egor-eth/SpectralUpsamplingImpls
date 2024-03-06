#include "argparse.h"
#include <upsample/upsamplers.h>
#include <imageutil/image.h>
#include <spec/basic_spectrum.h>
#include <spec/spectral_util.h>
#include <spec/conversions.h>
#include <internal/common/format.h>
#include <internal/common/util.h>
#include <chrono>
#include <stdexcept>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <memory>
#include <unordered_map>

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;

using namespace spec;
namespace fs = std::filesystem;
std::unordered_map<std::string, const LazyPtr<IUpsampler> *> upsampler_by_name;

void fill_method_map() {
    upsampler_by_name["glassner"] = &spec::upsamplers::glassner;
    upsampler_by_name["smits"] = &spec::upsamplers::smits;
    upsampler_by_name["sigpoly"] = &spec::upsamplers::sigpoly;
}

const IUpsampler *get_upsampler_by_name(const std::string &method_name)
{
    const auto it = upsampler_by_name.find(method_name);
    if(it == upsampler_by_name.end()) return nullptr;
    return it->second->get();
}

//https://stackoverflow.com/questions/874134/find-out-if-string-ends-with-another-string-in-c
inline bool string_ends_with(const std::string &value, const std::string &ending)
{
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

int downsample(const Args &args) {

    std::cout << "Loading file..." << std::endl;
    ISpectralImage::ptr spec_img;
    ISpectrum::ptr spec;
    ISpectrum::csptr illum;
    const std::string output_path = (fs::path(args.output_dir) / fs::path(*args.output_name)).string();

    if(!spec::util::load_spectral_image(args.input_path, spec_img, illum)) {
        if(!spec::util::load_spectrum(args.input_path, spec, illum)) {
            std::cerr << "Unknown file format" << std::endl;
            return 2;
        }        
        std::cout << "Converting spectum to RGB..." << std::endl;
        vec3 downsampled_rgb = xyz2rgb(spectre2xyz(*spec, *illum));
        std::ofstream output{output_path + ".txt"};
        output << downsampled_rgb.x << " " << downsampled_rgb.y << " " << downsampled_rgb.z << std::endl; 
        return 0;
    }

    const int w = spec_img->get_width();
    const int h = spec_img->get_height();
    Image img{w, h};


    std::cout << "Converting spectral image to png..." << std::endl;
    init_progress_bar(w * h, 10);

    for(int j = 0; j < h; ++j) {
        for(int i = 0; i < w; ++i) {
            img.at(i, j) = Pixel::from_vec3(xyz2rgb(spectre2xyz(spec_img->at(i, j), *illum)));
            print_progress(j * w + i);
        }
    }
    finish_progress_bar();

    img.save(output_path + ".png");

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
