#include <iostream>
#include <glog/logging.h>
#include "functions.h"
#include "lutworks.h"
#include <fstream>
#include "serialization/csv.h"
#include "common/format.h"
#include "serialization/parsers.h"

int main(int argc, char **argv)
{   
    if(argc < 2) {
        return 1;
    }

    google::InitGoogleLogging(argv[0]);

    /*std::ifstream initial_x_file{"resources/initial_x.txt"};

    const auto parse_result = spec::csv::load_as_vector<double, double, double>(initial_x_file, ' ')[0];
    vec3d init_x{std::get<0>(parse_result), std::get<1>(parse_result), std::get<2>(parse_result)};
    const vec3 color = spec::Pixel::from_argb(std::stoi(argv[1], nullptr, 16)).to_vec3();

    vec3d solution;
   // auto stable = find_stable(init_x, solution, 200, 20, 1e-3);
    //auto stable = find_stable_i(color, solution, 40, 20, 1e-3);
    vec3d result = solve_for_rgb(color, init_x);
    vec3 downsampled = spec::xyz2rgb(spec::sigpoly2xyz(result.x, result.y, result.z));
    if(stable) {
        std::cout 
            << "Stable color is: " << sre
            << " (result: " << " with downsample: " << stable->second << ") "
            << solution << std::endl;
    }
    else {
        std::cout << "Not found" << std::endl;
    } * /
    std::cout 
         << "Color is: " << color
         << " result is: " << downsampled
         << std::endl;
    */

    int zeroed_idx = spec::parse<int>(argv[1]);

    LUT lut = generate_lut(zeroed_idx, 1);

    std::ofstream output{spec::format("output/lut_%d.slf", zeroed_idx)};
    write_header(output);
    write_lut(output, lut);


    return 0;
} 
