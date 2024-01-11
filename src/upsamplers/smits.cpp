#include "smits.h"
#include "common/progress.h"
#include "math/math.h"
#include "spec/basic_spectrum.h"

namespace spec {

    namespace {

        constexpr Float WAVELENGHTS[] = {
            397, 431, 465, 499, 533,
            567, 601, 635, 669, 703
        };

        constexpr Float WHITE_SPECTRUM[] = {
            1.0000f, 1.0000f, 0.9999f, 0.9993f, 0.9992f,
            0.9998f, 1.0000f, 1.0000f, 1.0000f, 1.0000f
        };

        constexpr Float CYAN_SPECTRUM[] = {
            0.9710f, 0.9426f, 1.0007f, 1.0007f, 1.0007f,
            1.0007f, 0.1564f, 0.0000f, 0.0000f, 0.0000f
        };

        constexpr Float MAGENTA_SPECTUM[] = {
            1.0000f, 1.0000f, 0.9685f, 0.2229f, 0.0000f,
            0.0458f, 0.8369f, 1.0000f, 1.0000f, 0.9959f
        };

        constexpr Float YELLOW_SPECTUM[] = {
            0.0001f, 0.0000f, 0.1088f, 0.6651f, 1.0000f,
            1.0000f, 0.9996f, 0.9586f, 0.9685f, 0.9840f
        };

        constexpr Float RED_SPECTRUM[] = {
            0.1012f, 0.0515f, 0.0000f, 0.0000f, 0.0000f, 
            0.0000f, 0.8325f, 1.0149f, 1.0149f, 1.0149f
        };

        constexpr Float GREEN_SPECTRUM[] = {
            0.0000f, 0.0000f, 0.0273f, 0.7937f, 1.0000f,
            0.9418f, 0.1719f, 0.0000f, 0.0000f, 0.0025f
        };

        constexpr Float BLUE_SPECTRUM[] = {
            1.0000f, 1.0000f, 0.8916f, 0.3323f, 0.0000f,
            0.0000f, 0.0003f, 0.0369f, 0.0483f, 0.0496f
        }; 

        constexpr size_t SPECTRUM_SIZE = sizeof(WHITE_SPECTRUM) / sizeof(Float); 



        void add_array_multiplied(BasicSpectrum &s, const Float mul, const Float *ptr)
        {
            for(unsigned i = 0; i < SPECTRUM_SIZE; ++i) {
                int wl = WAVELENGHTS[i];
                s.get_or_create(wl) += mul * (*(ptr++));
            }
        }

        void upsample_pixel_to(const Pixel &pixel, BasicSpectrum &s) 
        {
            s = BasicSpectrum();
            vec3 rgb = pixel.to_vec3();

            if(pixel.r <= pixel.g && pixel.r <= pixel.b) {
                add_array_multiplied(s, rgb[0], WHITE_SPECTRUM);

                if(pixel.g <= pixel.b) {
                    add_array_multiplied(s, rgb[1] - rgb[0], CYAN_SPECTRUM);
                    add_array_multiplied(s, rgb[2] - rgb[1], BLUE_SPECTRUM);
                }
                else {
                    add_array_multiplied(s, rgb[2] - rgb[0], CYAN_SPECTRUM);
                    add_array_multiplied(s, rgb[1] - rgb[2], GREEN_SPECTRUM);
                }
            }
            else if(pixel.g <= pixel.r && pixel.g <= pixel.b) {
                add_array_multiplied(s, rgb[1], WHITE_SPECTRUM);

                if(pixel.r <= pixel.b) {
                    add_array_multiplied(s, rgb[0] - rgb[1], MAGENTA_SPECTUM);
                    add_array_multiplied(s, rgb[2] - rgb[0], BLUE_SPECTRUM);
                }
                else {
                    add_array_multiplied(s, rgb[2] - rgb[1], MAGENTA_SPECTUM);
                    add_array_multiplied(s, rgb[0] - rgb[2], RED_SPECTRUM);
                }
            }
            else /*if(pixel.b <= pixel.r && pixel.b <= pixel.g)*/ {
                add_array_multiplied(s, rgb[2], WHITE_SPECTRUM);

                if(pixel.r <= pixel.g) {
                    add_array_multiplied(s, rgb[0] - rgb[2], YELLOW_SPECTUM);
                    add_array_multiplied(s, rgb[1] - rgb[0], GREEN_SPECTRUM);
                }
                else {
                    add_array_multiplied(s, rgb[1] - rgb[2], YELLOW_SPECTUM);
                    add_array_multiplied(s, rgb[0] - rgb[1], RED_SPECTRUM);
                }
            }
        }

    }



    ISpectrum::ptr SmitsUpsampler::upsample_pixel(const Pixel &src) const
    {
        BasicSpectrum *spectrum = new BasicSpectrum();
        upsample_pixel_to(src, *spectrum);
        return ISpectrum::ptr(spectrum);
    }

    ISpectralImage::ptr SmitsUpsampler::upsample(const Image &sourceImage) const
    {
        BasicSpectralImage *dest = new BasicSpectralImage(sourceImage.get_width(), sourceImage.get_height());
        init_progress_bar();
       
        const long img_size = sourceImage.get_width() * sourceImage.get_height();
        const double wh = 1.0 / img_size;

        for(unsigned i = 0; i < SPECTRUM_SIZE; ++i) {
            dest->add_wavelenght(WAVELENGHTS[i]);
        }

        const Pixel *ptr = sourceImage.raw_data();
        BasicSpectrum *s_ptr = dest->raw_data();
        for(int i = 0; i < img_size; ++i) {
            upsample_pixel_to(ptr[i], s_ptr[i]);
            print_progress((i + 1) * wh);
        }

        finish_progress_bar();
        return ISpectralImage::ptr(dest);
    }
}