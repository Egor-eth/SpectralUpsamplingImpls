#include <spec/spectral_util.h>
#include <iostream>

using namespace spec;

int main()
{
    ISpectralImage::ptr img;
    spec::util::load("input/food_chili.hdr", img);

    spec::util::save("output", "food_chili", *img);
} 
