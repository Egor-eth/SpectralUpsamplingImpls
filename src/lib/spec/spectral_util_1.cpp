#include "spectral_util.h"
#include <fstream>
#include <stdexcept>
#include <memory>
#include <limits>
#include <filesystem>
#include <stb_image_write.h>
#include <nlohmannjson/json.hpp>
#include "common/format.h"
#include "common/constants.h"

namespace fs = std::filesystem;

using namespace spec;

namespace
{
    const std::string IMG_FILENAME_FORMAT = "w_" + FLOAT_FORMAT + ".png";
    const std::string SPD_OUTPUT_FORMAT = FLOAT_FORMAT + " " + FLOAT_FORMAT + "\n";

    void __to_stream(void *context, void *data, int size)
    {

        std::ostream *stream = reinterpret_cast<std::ostream *>(context);
        stream->write(reinterpret_cast<const char *>(data), size);
        stream->flush();
    }

    int write_png_to_stream(std::ostream &stream, int width, int height, int channels, const unsigned char *buf)
    {
        return stbi_write_png_to_func(__to_stream, reinterpret_cast<void *>(&stream), width, height, channels, buf, 0);
    }

    void normalize_and_convert_to_rgb(const BasicSpectralImage &img, unsigned char *dst, const std::vector<Float> &wavelenghts, int channels, Float &range_out, Float &min_val_out)
    {

        //calculate normalization data
        Float min_val = std::numeric_limits<Float>::max();
        Float max_val = std::numeric_limits<Float>::min();

        const BasicSpectrum *ptr;
        const BasicSpectrum *end = img.raw_data() + img.get_height() * img.get_width();
        for(ptr = img.raw_data(); ptr < end; ++ptr) {
            for(Float w : wavelenghts) {
                Float val = (*ptr)[w];
                if(val > max_val) max_val = val; 
                if(val < min_val) min_val = val;
            }
        }

        Float range = max_val - min_val;
        if(range < 1.0f) { //what about small values?
            range = 1.0f;
        }


        const BasicSpectrum *data = img.raw_data();
        //normalize and write to rgb buffer
        const int vector_size = wavelenghts.size();
        for(long i = 0; i < img.get_height() * img.get_width(); ++i) {
            for(int c = 0; c < vector_size; ++c) {
                Float w = data[i][wavelenghts[c]];
                Float w_norm = (w - min_val) / range;
                dst[i * channels + c] = static_cast<unsigned char>(w_norm * 255.999f);
            }
            for(int c = wavelenghts.size(); c < channels; ++c) {
                dst[i * channels + c] = 0; //zero additional channels
            }
        }

        range_out = range;
        min_val_out = min_val;
    }
}

using json = nlohmann::json;

namespace spectral
{

    void Metadata::save(std::ostream &stream) const
    {
        json meta;
        meta["width"] = width;
        meta["height"] = height;

        meta["format"] = format;

        json wavelenghts_array = json::array();
        for(const auto &metaentry : wavelenghts) {
            json entry;
            entry["filename"] = metaentry.filename;
            json targets = json::array();
            for(Float val : metaentry.targets)
                targets.insert(targets.end(), val);

            entry["targets"] = targets;
            entry["norm_min_val"] = metaentry.norm_min_val;
            entry["norm_range"] = metaentry.norm_range;
            wavelenghts_array.insert(wavelenghts_array.end(), entry);
        }
        meta["wavelenghts"] = wavelenghts_array;


        stream << meta.dump(2);
        stream.flush();
    }

    void Metadata::load(std::istream &stream)
    {
        (void) stream;
    }

    void save_spd(const std::string &path, const BasicSpectrum &spectre)
    {
        std::ofstream file(path, std::ios::trunc);
        if(!file) throw std::runtime_error("Cannot open file");

        for(const auto &e : spectre.get_map()) {
            file << format(SPD_OUTPUT_FORMAT, e.first, e.second);
        }

        file.flush();
    }

    void save_wavelenghts_to_png_multichannel(std::ostream &stream, const BasicSpectralImage &img, const std::vector<Float> &wavelenghts, SavingResult &res, int requested_channels)
    {
        const int vector_size = wavelenghts.size();
        if(vector_size < 1 || vector_size > 4) throw std::invalid_argument("Illegal wavelenghts size");

        int channels = vector_size > requested_channels ? vector_size : requested_channels;
        const int width = img.get_width();
        const int height = img.get_height();


        std::unique_ptr<unsigned char[]> buf{new unsigned char[width * height * channels]};
        if(!buf){
            res.success = false;
            throw std::bad_alloc();
        }

        normalize_and_convert_to_rgb(img, buf.get(), wavelenghts, channels, res.norm_range, res.norm_min);

        int code = write_png_to_stream(stream, width, height, channels, buf.get());
        if(!code) {
            res.success = false;
            throw std::runtime_error("Error saving to file");
        }
        res.channels_used = channels;
    }

    void save_wavelenght_to_png1(std::ostream &stream, const BasicSpectralImage &img, Float wavelenght, SavingResult &res)
    {
        const int width = img.get_width();
        const int height = img.get_height();
        std::unique_ptr<unsigned char[]> buf{new unsigned char[width * height]};
        if(!buf) {
            res.success = false;
            throw std::bad_alloc();
        }

        normalize_and_convert_to_rgb(img, buf.get(), {wavelenght}, 1, res.norm_range, res.norm_min);

        int code = write_png_to_stream(stream, width, height, 1, buf.get());
        if(!code) {
            res.success = false;
            throw std::runtime_error("Error saving to file");
        }
        res.channels_used = 1;
    }

    bool save_as_png1(const BasicSpectralImage &image, const std::string &dir, const std::string &meta_filename) {

        Metadata metadata;
        metadata.width = image.get_width();
        metadata.height = image.get_height();

        metadata.format = "png1";

        const fs::path dir_path{dir};
        fs::create_directories(dir_path);
        
        SavingResult saving_result;
        for(Float w : image.get_wavelenghts()) {
            //save as 1-channel png
            std::string filename = format(IMG_FILENAME_FORMAT, w);
            fs::path img_path = dir_path / filename;
            std::fstream file(img_path, std::ios::out | std::ios::binary | std::ios::trunc);
            try {
                save_wavelenght_to_png1(file, image, w, saving_result);
            } catch(...) {
                file.close();
                throw;
            }
            //add entry to metadata
            metadata.wavelenghts.push_back(MetadataEntry{filename, {w}, saving_result.norm_min, saving_result.norm_range});
            file.close();
        }
        //save metadata
        std::fstream meta_file(dir_path / meta_filename, std::ios::out | std::ios::trunc);
        metadata.save(meta_file);
        meta_file.close();

        return true;
    }

    bool save_as_png3(const BasicSpectralImage &image, const std::string &dir, const std::string &meta_filename)
    {
        (void) image;
        (void) dir;
        (void) meta_filename;
        return false;
    }

}