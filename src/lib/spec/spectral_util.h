#ifndef COLOR_SPECTRAL_UTIL_H
#define COLOR_SPECTRAL_UTIL_H
#include <string>
#include <vector>
#include <ostream>
#include <istream>
#include "basic_spectrum.h"
#include "sigpoly_spectrum.h"

namespace spec
{

    namespace util {
        const std::string META_FILENAME = "meta.json";

        struct SavingResult
        {
            bool success;
            int channels_used;
            Float norm_min;
            Float norm_range;
        };

        struct MetadataEntry
        {
            std::string filename;
            std::vector<Float> targets;
            Float norm_min_val;
            Float norm_range;
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

        void save_spd(const std::string &path, const BasicSpectrum &spectre);

        BasicSpectrum load_spd(const std::string &path);

        /**
         *  Saves specified wavelenghts of spectral image in multichannel png file (up to 4 channels).
         * Writes saving data to res. If wavelenghts is empty or contains more than 4 
         * elements exception is thrown.
         */
        void save_wavelenghts_to_png_multichannel(std::ostream &stream, const BasicSpectralImage &img, const std::vector<Float> &wavelenghts, SavingResult &res, int requested_channels = 0);

        void save_wavelenght_to_png1(std::ostream &stream, const BasicSpectralImage &img, Float wavelenght, SavingResult &res);

        bool save_as_png1(const BasicSpectralImage &image, const std::string &dir, const std::string &meta_filename = META_FILENAME);

        bool save_as_png3(const BasicSpectralImage &image, const std::string &dir, const std::string &meta_filename = META_FILENAME);

    //    bool save_as_png3(const BasicSpectralImage &image, const std::string &dir, const std::string &meta_filename = META_FILENAME);


        bool save_sigpoly(const std::string path, const SigPolySpectrum &spectrum);

    }
}

#endif