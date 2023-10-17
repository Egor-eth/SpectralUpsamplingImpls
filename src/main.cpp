#include <string>
#include <unistd.h>
#include "imageutil/image.h"
#include "imageutil/spectral_image.h"
#include "glm/glm.hpp"
#include <iostream>
#include "upsamplers/naive_upsampler.h"


// -c (hex): use color code instead of texture
// 
int main(int argc, char **argv)
{
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
            std::cerr << "Unknown argument" << std::endl; 
            break;
        }
    }  
    if(!has_input) {
        std::cerr << "No input data" << std::endl;
        return 1;
    }
    if(!has_method) {
        std::cerr << "No method specified" << std::endl;
        return 1;
    }

    if(optind < argc) {
        output_path = argv[optind];
    }


    SpectralImage img;

    IUpsampler *upsampler = new NaiveUpsampler();

    upsampler->upsample(image, img);
    img.save(output_path);

    delete upsampler;
} 
