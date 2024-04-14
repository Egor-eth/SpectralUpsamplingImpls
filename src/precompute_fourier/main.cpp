#include "functions.h"
//#include "lutworks.h"
#include <spec/conversions.h>
#include <internal/serialization/parsers.h>
#include <internal/serialization/csv.h>
#include <internal/common/format.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <glog/logging.h>

const std::string EMISS_LUT_FILENAME = "output/f_emission_lut.eflf";

int main(int argc, char **argv)
{   
    (void) argc;
    google::InitGoogleLogging(argv[0]);


    std::ifstream file{"input/dataset.csv"};
/*
    std::vector<Float> wavelenghts = std::get<0>(*csv::parse_line_m<Float>(file));
    auto data = csv::load_as_vector_m<Float>(file);

    const std::vector<Float> &values = std::get<0>(data[1]);

    vec3 color = xyz2rgb(_spectre2xyz(wavelenghts, values));

    vec3 target_color_gt = color + vec3(5.0f / 255.0f);

    std::vector<double> momentsd = adjust_and_compute_moments(target_color_gt, wavelenghts, values);
    std::vector<Float> moments(M + 1);
    for(int i = 0; i <= M; ++i) moments[i] = Float(momentsd[i]);

    std::vector<Float> phases = math::wl_to_phases(wavelenghts);
    std::vector<Float> new_values = math::mese(phases, moments);

    vec3 target_color_out = xyz2rgb(_spectre2xyz(wavelenghts, new_values));

    std::cout << target_color_out << "<-" << target_color_gt << std::endl;
*/
    
    //int zeroed_idx = spec::parse<int>(argv[1]);

    SigpolyLUT lut = generate_lut(zeroed_idx, 4, 24);
    std::string output_path = spec::format(EMISS_LUT_FILENAME, zeroed_idx);
    std::ofstream output{output_path};

    std::cout << "Writing data to " << output_path << "." << std::endl;
    write_header(output);
    write_lut(output, lut);
    std::cout << "Successfully written data." << std::endl;

    return 0;
} 
