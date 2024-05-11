#include <spec/spectral_util.h>
#include <spec/basic_spectrum.h>
#include <spec/conversions.h>
#include <spec/metrics.h>
#include <internal/serialization/csv.h>
#include <internal/math/math.h>
#include <internal/common/util.h>
#include <upsample/glassner_naive.h>
#include <upsample/smits.h>
#include <upsample/sigpoly.h>
#include <iomanip>
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

void dataset_reupsample(const IUpsampler &upsampler, const std::string &method_name)
{
    std::vector<BasicSpectrum> in_spectra;
    load_spec_ds("input/spectrum_m.csv", in_spectra);
    std::vector<BasicSpectrum> out_spectra(in_spectra.size());

    std::vector<vec3> in_xyz;
    load_xyz_ds("input/xyz_m.csv", in_xyz);

    std::ofstream output_file("output/comparsion_ds_result_" + method_name + ".csv");
    std::ofstream result_spectra_file("output/comparsion_ds_converted_" + method_name + ".csv");

    int successful = 0;
    std::vector<double> losses;
    std::vector<Float> spec_maes(in_spectra.size());
    std::vector<Float> spec_sams(in_spectra.size());

    for(unsigned i = 0; i < in_xyz.size(); ++i) {
        vec3 rgb = xyz2rgb(in_xyz[i]);
        ISpectrum::ptr spec = upsampler.upsample_pixel(Pixel::from_vec3(rgb));

        vec3 lab = xyz2cielab(spectre2xyz(*spec));
        const Float deltaE = vec3::distance(lab, xyz2cielab(in_xyz[i]));
        successful += deltaE < 2.333f;
        losses.push_back(deltaE);

        spec_maes[i] = metrics::mae(*spec, in_spectra[i]);
        spec_sams[i] = metrics::sam(*spec, in_spectra[i]);
        out_spectra[i] = util::convert_to_spd(*spec);

       // std::cout << xyz << " " << in_xyz[i] << std::endl;
    }

    auto [minde, maxde] = minmax_element(losses.begin(), losses.end());
    auto [minsam, maxsam] = minmax_element(spec_sams.begin(), spec_sams.end());

    output_file << std::setprecision(8)
                << "# Overall MSE DeltaE loss: " << std::accumulate(losses.begin(), losses.end(), 0.0f) / in_xyz.size() << ";\n"
                << "# Min DeltaE: " <<  *minde << ", max DeltaE: " << *maxde << "\n"
                << "# Median DeltaE: " << losses[losses.size() / 2] << ";\n"
                << "# Unrecognizable difference in " << successful << "/" << in_xyz.size() << " values.\n#" << std::endl;

    output_file << "# Average spectra MAE is " << std::accumulate(spec_maes.begin(), spec_maes.end(), 0.0f) / in_xyz.size() << ";\n"
                << "# Min SAM: " << *minsam << ", max SAM: " << *maxsam << ".\n#\n"
                << "DeltaE,Spectral MAE,SAM" << std::endl;

    for(unsigned i = 0; i < in_xyz.size(); ++i) {
        output_file << losses[i] << "," << spec_maes[i] << "," << spec_sams[i] << "\n";
    }
    output_file.flush();
    output_file.close();

    result_spectra_file << std::setprecision(16);
    for(int i = WAVELENGHTS_START; i < WAVELENGHTS_END; i += WAVELENGHTS_STEP) {
        result_spectra_file << i << ",";
    }
    result_spectra_file << WAVELENGHTS_END << std::endl;

    for(const BasicSpectrum &sp : out_spectra) {
        for(int i = WAVELENGHTS_START; i < WAVELENGHTS_END; i += WAVELENGHTS_STEP) {
            result_spectra_file << sp(i) << ",";
        }
        result_spectra_file << sp(WAVELENGHTS_END) << std::endl;
    }
}

void img_reupsample(const IUpsampler &upsampler, const std::string &path, const std::string &method)
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
        dataset_reupsample(*upsampler, method);
        return 0;
    }
    if(!strcmp(argv[2], "im")) {
        if(argc >= 4) {
            img_reupsample(*upsampler, std::string(argv[3]), method);
            return 0;
        }
        return 1;
    }

} 
