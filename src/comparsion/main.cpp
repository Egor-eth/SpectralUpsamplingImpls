#include <spec/spectral_util.h>
#include <spec/basic_spectrum.h>
#include <spec/conversions.h>
#include <internal/serialization/csv.h>
#include <internal/math/math.h>
#include <upsample/glassner_naive.h>
#include <numeric>
#include <iostream>
#include <fstream>

using namespace spec;

void load_spec_ds(const std::string &path, std::vector<BasicSpectrum> &spectra) {
    std::ifstream file_in{path};

    auto header = *csv::parse_line_m<spec::Float, csv::skip>(file_in);

    auto table = csv::load_as_vector_m<spec::Float, csv::skip>(file_in);
    file_in.close();

    const std::vector<spec::Float> &wavelenghts = std::get<0>(header);
    for(const auto &entry : table) {
        BasicSpectrum spectrum;
        const std::vector<spec::Float> &values = std::get<0>(entry);
        for(unsigned i = 0; i < wavelenghts.size(); ++i) {
            spectrum.set(wavelenghts[i], values[i]);
        }
        spectra.push_back(spectrum);
    }
}

void load_xyz_ds(const std::string &path, std::vector<vec3> &data) {
    std::ifstream file_in{path};

    auto table = csv::load_as_vector<csv::skip, Float, Float, Float>(file_in, ',', 1);
    file_in.close();

    for(const auto &entry : table) {
        BasicSpectrum spectrum;
        data.push_back({std::get<1>(entry), std::get<2>(entry), std::get<3>(entry)});
    }
}


int main()
{

   // std::vector<BasicSpectrum> spectra;
    std::vector<vec3> xyz_values;
    //load_spec_ds("input/spectrum_m.csv", spectra);
    load_xyz_ds("input/xyz_m.csv", xyz_values);

    int successful = 0;
    std::vector<double> losses;
    GlassnerUpsampler upsampler{};


    for(unsigned i = 0; i < xyz_values.size(); ++i) {
        vec3 rgb = xyz2rgb(xyz_values[i]);
        ISpectrum::ptr spec = upsampler.upsample_pixel(Pixel::from_vec3(rgb));

        vec3 lab = xyz2cielab(spectre2xyz(*spec));
        Float distance = vec3::distance(lab, xyz2cielab(xyz_values[i]));
        successful += distance == 0.0;
        losses.push_back(distance);

       // std::cout << xyz << " " << xyz_values[i] << std::endl;
    }

    for(double d : losses) {
        std::cout << d << std::endl;
    }

    std::cout << "Overall MSE loss: " << std::accumulate(losses.begin(), losses.end(), 0.0) / xyz_values.size() << std::endl;

} 
