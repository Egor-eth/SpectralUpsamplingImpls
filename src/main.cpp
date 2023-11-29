#include <unistd.h>
#include <chrono>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <memory>
#include <unordered_map>
#include "upsamplers/glassner_naive.h"
#include "upsamplers/smits.h"
#include "imageutil/image.h"
#include "color/spectral_image.h"
#include "color/conversions.h"
#include "common/format.h"

namespace {

    std::unordered_map<std::string, std::unique_ptr<IUpsampler>> upsampler_by_method;

    void fill_method_map() {
        upsampler_by_method["glassner"] = std::unique_ptr<IUpsampler>(new GlassnerUpsampler());
        upsampler_by_method["smits"] = std::unique_ptr<IUpsampler>(new SmitsUpsampler());
    }

}
const IUpsampler *get_upsampler_for_method(const std::string &method_name)
{
    const auto it = upsampler_by_method.find(method_name);
    if(it == upsampler_by_method.end()) return nullptr;
    return it->second.get();
}

struct Args {
    std::string output_path;
    std::string method;
    Image image;
    bool single_color;
};

int parse_args(int argc, char **argv, Args &args)
{
    bool has_input = false;
    bool has_method = false;
    int c;
    while((c = getopt(argc, argv, "c:f:m:")) != -1) {
        switch(c) {
        case 'c':
            if(has_input) return 1;
            args.image = Image(1, 1);
            args.image.at(0, 0) = Pixel::from_argb(std::stoi(optarg, nullptr, 16));
            has_input = true;
            args.single_color = true;
            break;
        case 'f':
            if(has_input) return 1;
            args.image = Image(optarg);
            has_input = true;
            args.single_color = false;
            break;
        case 'm':
            if(has_method) return 1;
            args.method = optarg;
            has_method = true;
            break;
        case '?':
            std::cerr << "[!] Unknown argument." << std::endl; 
            break;
        }
    }  
    if(!has_input) {
        std::cerr << "[!] No input data." << std::endl;
        return 1;
    }
    if(!has_method) {
        std::cerr << "[!] No method specified." << std::endl;
        return 1;
    }

    if(optind < argc) {
        args.output_path = argv[optind];
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
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;

    Args args;
    if(parse_args(argc, argv, args)) return 1;

    SpectralImage spectral_img;

    const IUpsampler *upsampler = get_upsampler_for_method(args.method);
    if(upsampler == nullptr) {
        std::cerr << "[!] Unknown method." << std::endl;
        return 1;
    }
    try {
        std::cout << "Converting image to spectral with " << args.method << " method..." << std::endl;
        auto t1 = high_resolution_clock::now();
        upsampler->upsample(args.image, spectral_img);
        auto t2 = high_resolution_clock::now();
        std::cout << "Upsampling took " << duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms." << std::endl;
        if(args.single_color) {

            vec3 downsampled_rgb = xyz2rgb(spectre2xyz(spectral_img.at(0, 0)));
            std::cout
                << format("Downsampled RGB: (%d, %d, %d)",
                    static_cast<int>(downsampled_rgb.x * 255.99),
                    static_cast<int>(downsampled_rgb.y * 255.99),
                    static_cast<int>(downsampled_rgb.z * 255.99))
                << std::endl;
        }
        std::cout << "Saving..." << std::endl;
        if(!spectral_img.save(args.output_path)) {
            std::cerr << "[!] Error saving image." << std::endl;
        }
    } catch (std::exception &ex) {
        std::cerr << "[!] " << ex.what() << std::endl;
        return 1;
    }
    return 0;
} 
