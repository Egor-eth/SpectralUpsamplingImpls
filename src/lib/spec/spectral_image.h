#ifndef COLOR_SPECTRAL_IMAGE_H
#define COLOR_SPECTRAL_IMAGE_H
#include <set>
#include "imageutil/base_image.h"
#include "spectrum.h"

class SpectralImage;

/**
 * Formats: 
 *          png1, json          - monochrnomatic png for each wavelenght and metadata file (default)
 *          png1_zip, zip       - same as 'png1', but packed in zip archive
 *          png3, json3         - 3 channel png for each triplet of wavelenghts and metadata file
 *          png3_zip            - same as 'png3', but packet in zip archive
 *          one_spectre         - saves spectre of (0, 0) pixel in text format
 * 
 *  If format is not specified (empty string is supplied), format will be choosen from name of file. 
 * If 'png1', or 'png3' format is used, path is interpreted as directory to save those files to,
 * otherwise, path is considered to be filename. If 'json' or 'json3' format is used, all
 * correspinding pngs are saved in the same directory as file. 
 */
class SpectralSaver
{
public:
    bool operator()(const std::string &path, const BaseImage<Spectrum, void, SpectralSaver> &image_, const std::string &format = "") const;
};

class SpectralImage : public BaseImage<Spectrum, void, SpectralSaver>
{
public:
    SpectralImage()
        : BaseImage<Spectrum, void, SpectralSaver>(), wavelenghts() {}

    SpectralImage(int w, int h)
        : BaseImage<Spectrum, void, SpectralSaver>(w, h), wavelenghts() {}  

    SpectralImage(int w, int h, const Spectrum &p)
        : BaseImage<Spectrum, void, SpectralSaver>(w, h, p), wavelenghts(p.get_wavelenghts()) {} 

    SpectralImage(const SpectralImage &img)
        : BaseImage<Spectrum, void, SpectralSaver>(img), wavelenghts(img.wavelenghts) {}

    SpectralImage(SpectralImage &&image)
        : BaseImage<Spectrum, void, SpectralSaver>(std::move(image)), wavelenghts(std::move(image.wavelenghts)) {}

    SpectralImage &operator=(SpectralImage &&other)
    {
        if(this != &other) { 
            BaseImage<Spectrum, void, SpectralSaver>::operator=(std::move(other));
            wavelenghts = std::move(other.wavelenghts);
        }
        return *this;
    }

    void add_wavelenght(Float w);

    void remove_wavelenght(Float w);

    inline const std::set<Float> &get_wavelenghts() const
    {
        return wavelenghts;
    }

    void set_wavelenghts(const std::set<Float> &wl) {
        wavelenghts = wl;
    }

    void set_wavelenghts(std::set<Float> &&wl) {
        wavelenghts = std::move(wl);
    }

    bool validate() const;

private:
    std::set<Float> wavelenghts;
};


#endif 
