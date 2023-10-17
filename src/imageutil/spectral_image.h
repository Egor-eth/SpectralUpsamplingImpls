#ifndef IMAGEUTIL_SPECTRAL_IMAGE_H
#define IMAGEUTIL_SPECTRAL_IMAGE_H
#include <memory>
#include <unordered_set>
#include "constants.h"
#include "base_image.h"
#include "spectre.h"

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
    bool operator()(const std::string &path, const BaseImage<Spectre, void, SpectralSaver> &image_, const std::string &format = "") const;
};

class SpectralImage : public BaseImage<Spectre, void, SpectralSaver>
{
public:
    SpectralImage()
        : BaseImage<Spectre, void, SpectralSaver>() {}

    SpectralImage(int w, int h)
        : BaseImage<Spectre, void, SpectralSaver>(w, h) {}  

    SpectralImage(int w, int h, const Spectre &p)
        : BaseImage<Spectre, void, SpectralSaver>(w, h, p) {} 

    SpectralImage(const SpectralImage &img)
        : BaseImage<Spectre, void, SpectralSaver>(img) {}

    SpectralImage(SpectralImage &&image)
        : BaseImage<Spectre, void, SpectralSaver>(std::move(image)) {}

    SpectralImage &operator=(SpectralImage &&other)
    {
        BaseImage<Spectre, void, SpectralSaver>::operator=(std::move(other));
        return *this;
    }

private:
    std::shared_ptr<std::unordered_set<SpectreFloat>> wavelenghts;
};


#endif 
