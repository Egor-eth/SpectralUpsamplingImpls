#include <spec/spectral_util.h>
#include <spec/basic_spectrum.h>
#include <spec/conversions.h>
#include <internal/serialization/csv.h>
#include <internal/math/math.h>
#include <internal/common/util.h>
#include <upsample/glassner_naive.h>
#include <upsample/smits.h>
#include <upsample/sigpoly.h>
#include <filesystem>
#include <memory>
#include <numeric>
#include <iostream>
#include <cstring>
#include <fstream>
#include <chrono>

using namespace spec;
namespace fs = std::filesystem;

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;

std::unique_ptr<IUpsampler> get_upsampler_by_name(const std::string &method_name)
{
    IUpsampler *ptr = nullptr;
    if(method_name == "glassner") {
        ptr = new GlassnerUpsampler();
    }
    else if(method_name == "sigpoly") {
        ptr = new SigPolyUpsampler();
    }
    else if(method_name == "smits") {
        ptr = new SmitsUpsampler();
    }
    return std::unique_ptr<IUpsampler>(ptr);
}

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

void dataset_resample(const IUpsampler &upsampler)
{
    // std::vector<BasicSpectrum> spectra;
    std::vector<vec3> xyz_values;
    //load_spec_ds("input/spectrum_m.csv", spectra);
    load_xyz_ds("input/xyz_m.csv", xyz_values);

    int successful = 0;
    std::vector<double> losses;

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

void img_resample(const IUpsampler &upsampler, const std::string &path, const std::string &method)
{
    Image image_gt{path};
    std::cout << "Upsampling " << path << std::endl;
    auto t1 = high_resolution_clock::now();
    ISpectralImage::ptr spectral_img = upsampler.upsample(image_gt);
    auto t2 = high_resolution_clock::now();
    std::cout << "Upsampling took " << duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms." << std::endl;

    int w = spectral_img->get_width(), h = spectral_img->get_height();
    Image image{w, h};

    std::cout << "Downsampling" << std::endl;
    for(int j = 0; j < h; ++j) {
        for(int i = 0; i < w; ++i) {
            image.at(i, j) = Pixel::from_vec3(xyz2rgb(spectre2xyz(spectral_img->at(i, j))));
        }
    }
    image.save((fs::path("output") / "tests") / (fs::path(path).stem().string() + "_" + method + ".png"));
}

int main(int argc, char **argv)
{
    if(argc <= 2) return 1;
    const std::string method = argv[1];
    std::unique_ptr<IUpsampler> upsampler = get_upsampler_by_name(method);

    if(!strcmp(argv[2], "ds")) {
        dataset_resample(*upsampler);
        return 0;
    }
    if(!strcmp(argv[2], "im")) {
        if(argc >= 4) {
            img_resample(*upsampler, std::string(argv[3]), method);
            return 0;
        }
        return 1;
    }

} 
