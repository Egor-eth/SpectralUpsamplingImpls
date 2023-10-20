#include <unistd.h>
#include <chrono>
#include <stdexcept>
#include <iostream>
#include <memory>
#include <unordered_map>
#include "upsamplers/naive_upsampler.h"
#include "imageutil/image.h"
#include "imageutil/spectral_image.h"

namespace {

    std::unordered_map<std::string, std::unique_ptr<IUpsampler>> upsampler_by_method;

    void fill_method_map() {
        upsampler_by_method["naive"] = std::unique_ptr<IUpsampler>(new NaiveUpsampler());
    }

}
const IUpsampler *get_upsampler_for_method(const std::string &method_name)
{
    const auto it = upsampler_by_method.find(method_name);
    if(it == upsampler_by_method.end()) return nullptr;
    return it->second.get();
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


    bool has_input = false;
    bool has_method = false;
    Image image;
    std::string output_path;
    std::string method;
    int c;
    while((c = getopt(argc, argv, "c:f:m:")) != -1) {
        switch(c) {
        case 'c':
            if(has_input) return 1;
            image = Image(1, 1);
            image.at(0, 0) = Pixel::from_argb(std::stoi(optarg, nullptr, 16));
            has_input = true;
            break;
        case 'f':
            if(has_input) return 1;
            image = Image(optarg);
            has_input = true;
            break;
        case 'm':
            if(has_method) return 1;
            method = optarg;
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
        output_path = argv[optind];
    }


    SpectralImage spectral_img;

    const IUpsampler *upsampler = get_upsampler_for_method(method);
    if(upsampler == nullptr) {
        std::cerr << "[!] Unknown method." << std::endl;
        return 1;
    }
    try {
        std::cout << "Converting image to spectral with " << method << " method..." << std::endl;
        auto t1 = high_resolution_clock::now();
        upsampler->upsample(image, spectral_img);
        auto t2 = high_resolution_clock::now();
        std::cout << "Upsampling took " << duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms. Saving..." << std::endl;
        if(!spectral_img.save(output_path)) {
            std::cerr << "[!] Error saving image." << std::endl;
        }
    } catch (std::exception &ex) {
        std::cerr << "[!] " << ex.what() << std::endl;
        return 1;
    }
    return 0;
} 
