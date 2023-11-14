#include "spectral_image.h"
#include <stdexcept>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "spectral_util.h"
#include "common/constants.h"
#include "common/format.h"

namespace fs = std::filesystem;

namespace 
{
    enum SaveFormat
    {
        UNKNOWN = -1,
        PNG1 = 0,
        PNG3,
        PNG1_JSON = 0xf000,
        PNG1_ZIP,
        PNG3_JSON,
        PNG3_ZIP,
        ONE_SPECTRE
    };

    const std::unordered_map<std::string, SaveFormat> str_to_format{
        {"png1", PNG1},
        {"json", PNG1_JSON},
        {"zip", PNG1_ZIP},
        {"png1_zip", PNG1_ZIP},
        {"png3", PNG3},
        {"json3", PNG3_JSON},
        {"png3_zip", PNG3_ZIP},
        {"one_spectre", ONE_SPECTRE},
        {"spd", ONE_SPECTRE}
    };

    inline void filecheck(const std::string &path, bool &is_directory, bool &exists)
    {
        auto p = fs::path(path);
        exists = fs::exists(p);
        if(exists) {
            is_directory = path.back() == fs::path::preferred_separator && fs::is_directory(p);
        }
    }

    inline SaveFormat get_type_from_path(const std::string &path)
    {
        fs::path p = path;
        std::string extension = p.extension();
        if(extension.empty()) return PNG1;
        else if(extension == ".json") return PNG1_JSON;
        else if(extension == ".zip") return PNG1_ZIP;
        else if(extension == ".spd") return ONE_SPECTRE;
        return UNKNOWN;
    }

    inline SaveFormat get_type(const std::string &path, const std::string &format)
    {
        auto it = str_to_format.find(format);
        if(it != str_to_format.end()) {
            return it->second;
        } else {
            return get_type_from_path(path);
        }
    }

}

bool SpectralSaver::operator()(const std::string &path, const BaseImage<Spectre, void, SpectralSaver> &image_, const std::string &format) const
{
    const SpectralImage &image = static_cast<const SpectralImage &>(image_); // Bad practice :(
    if(image.get_width() == 1 && image.get_height() == 1) {
        spectral::save_single_spd(path, image.at(0, 0));
    } else {
        if(!image.validate()) {
            std::cerr << "Could not validate spectral image" << std::endl;
            return false;
        }

        bool is_directory;
        bool exists;
        filecheck(path, is_directory, exists);

        SaveFormat type = get_type(path, format);
        std::cerr << type << std::endl;

        std::string target_directory;
        std::string target_file;
        if(is_directory && !(type & 0xf000)) {
            target_directory = path;
        } else {
            target_directory = fs::path(path).remove_filename().string();
            target_file = fs::path(path).filename();
        }

        std::cerr << "Target file: " << target_file << std::endl;
        std::cerr << "Dir: " << target_directory << std::endl;
        try {
            switch(type) {
            case PNG1:
                target_file = spectral::META_FILENAME;
                [[fallthrough]];
            case PNG1_JSON:
                return spectral::save_as_png1(image, target_directory, target_file);
            case PNG3:
                target_file = spectral::META_FILENAME;
                [[fallthrough]];
            case PNG3_JSON:
                return spectral::save_as_png3(image, target_directory, target_file);
            default:
                return false;
            }
        } catch (std::runtime_error &) {
            return false;
        }

    }
    return true;
}

template class BaseImage<Spectre, void, SpectralSaver>;

void SpectralImage::add_wavelenght(SpectreFloat w)
{
    wavelenghts.insert(w);
}

void SpectralImage::remove_wavelenght(SpectreFloat w)
{
    wavelenghts.erase(w);
}

bool SpectralImage::validate() const
{
    std::set<SpectreFloat> all_wavelenghts;
    for(long i = 0; i < width * height; ++i) {
        const auto &wl = data[i].get_wavelenghts();
        all_wavelenghts.insert(wl.begin(), wl.end());
    }
    return std::includes(wavelenghts.begin(), wavelenghts.end(),
                         all_wavelenghts.begin(), all_wavelenghts.end());
}