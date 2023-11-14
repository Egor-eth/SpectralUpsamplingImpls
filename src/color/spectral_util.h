#ifndef COLOR_SPECTRAL_UTIL_H
#define COLOR_SPECTRAL_UTIL_H
#include <string>
#include <vector>
#include <ostream>
#include <istream>
#include "spectral_image.h"

namespace spectral
{
    const std::string META_FILENAME = "meta.json";

    struct SavingResult
    {
        bool success;
        int channels_used;
        SpectreFloat norm_min;
        SpectreFloat norm_range;
    };

    struct MetadataEntry
    {
        std::string filename;
        std::vector<SpectreFloat> targets;
        SpectreFloat norm_min_val;
        SpectreFloat norm_range;
    };

    struct Metadata
    {
        int width;
        int height;
        std::string format;
        std::vector<MetadataEntry> wavelenghts;

        void save(std::ostream &stream) const;
        void load(std::istream &stream);
    };

    void save_single_spd(const std::string &path, const Spectre &spectre);

    /**
     *  Saves specified wavelenghts of spectral image in multichannel png file (up to 4 channels).
     * Writes saving data to res. If wavelenghts is empty or contains more than 4 
     * elements exception is thrown.
     */
    void save_wavelenghts_to_png_multichannel(std::ostream &stream, const SpectralImage &img, const std::vector<SpectreFloat> &wavelenghts, SavingResult &res, int requested_channels = 0);

    void save_wavelenght_to_png1(std::ostream &stream, const SpectralImage &img, SpectreFloat wavelenght, SavingResult &res);

    bool save_as_png1(const SpectralImage &image, const std::string &dir, const std::string &meta_filename = META_FILENAME);

    bool save_as_png3(const SpectralImage &image, const std::string &dir, const std::string &meta_filename = META_FILENAME);
}

#endif