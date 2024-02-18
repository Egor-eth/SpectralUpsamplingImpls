#include "spec/sp_lut.h"
#include "math/math.h"
#include <iostream>
#include <fstream>
#include "spec/sigpoly_spectrum.h"
#include "spec/conversions.h"

using namespace spec;

LUT load_from_file(const std::string &path)
{
    std::ifstream file{path};
    return LUT::load_from(file);
}

int main()
{
    LUT luts[3]{load_from_file("resources/sp_lut0.slf"), load_from_file("resources/sp_lut1.slf"), load_from_file("resources/sp_lut2.slf")};

    vec3 white = luts[2].eval(255, 255, 255 / 2);
    vec3 red = luts[2].eval(255, 0, 255 / 2);
    vec3 blue = luts[1].eval(255, 0, 255 / 2);
    SigPolySpectrum rb{white + red + blue};

    vec3 downsampled_rgb = xyz2rgb(spectre2xyz(rb));

    std::cout << downsampled_rgb << std::endl;
} 
