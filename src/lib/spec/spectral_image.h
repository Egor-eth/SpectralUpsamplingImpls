#ifndef COLOR_SPECTRAL_IMAGE_H
#define COLOR_SPECTRAL_IMAGE_H
#include <set>
#include "imageutil/base_image.h"
#include "basic_spectrum.h"

namespace spec {

    template <typename T>
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

    class BasicSpectralImage : public BaseImage<BasicSpectrum>
    {
    public:
        using BaseImage<BasicSpectrum>::BaseImage;

        //SpectralImage(const std::string &path);

        BasicSpectralImage(int w, int h, const BasicSpectrum &p)
            : BaseImage<BasicSpectrum>(w, h, p), wavelenghts(p.get_wavelenghts()) {} 

        BasicSpectralImage(const BasicSpectralImage &img)
            : BaseImage<BasicSpectrum>(img), wavelenghts(img.wavelenghts) {}

        BasicSpectralImage(BasicSpectralImage &&image)
            : BaseImage<BasicSpectrum>(std::move(image)), wavelenghts(std::move(image.wavelenghts)) {}

        BasicSpectralImage &operator=(BasicSpectralImage &&other)
        {
            if(this != &other) { 
                BaseImage<BasicSpectrum>::operator=(std::move(other));
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

         bool save(const std::string &path, const std::string &format = "") const;

        bool validate() const;

    private:
        std::set<Float> wavelenghts;
    };

}


#endif 
