#include <iostream>
#include <ceres/ceres.h>
#include <glog/logging.h>
#include "functions.h"
#include <fstream>
#include "common/csv.h"

using namespace ceres;

int main(int argc, char **argv)
{   
    if(argc < 2) {
        return 1;
    }

    google::InitGoogleLogging(argv[0]);


    std::ifstream initial_x_file{"resources/initial_x.txt"};

    const auto parse_result = spec::csv::load_as_vector<double, double, double>(initial_x_file, ' ')[0];
    vec3d init_x{std::get<0>(parse_result), std::get<1>(parse_result), std::get<2>(parse_result)};
    //const vec3 color = spec::Pixel::from_argb(std::stoi(argv[1], nullptr, 16)).to_vec3();

    vec3d solution;
    auto stable = find_stable(init_x, solution, 200, 20, 1e-3);
    //auto stable = find_stable_i(color, solution, 40, 20, 1e-3);
   // vec3d result = solveForRGB(color, init_x);
    if(stable) {
        std::cout 
            << "Stable color is: " << stable->first
            << " (result: " << " with downsample: " << stable->second << ") "
            << solution << std::endl;
    }
    else {
        std::cout << "Not found" << std::endl;
    }

    return 0;
} 
