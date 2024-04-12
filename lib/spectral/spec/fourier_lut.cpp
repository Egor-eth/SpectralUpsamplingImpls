#include <spec/fourier_lut.h>
#include <internal/serialization/binary.h>

namespace spec {

    namespace {

        inline bool validate_c(int a, int b, int c) {
            return a >= 0 && a <= 255 && b >= 0 && b <= 255 && c >= 0 && c <= 255;
        }

        inline int safe_int(int i, int size)
        {
            return i >= size ? size - 1 : i;
        }

        bool validate_header(std::istream &src)
        {
            uint64_t marker = binary::read<uint64_t>(src);
            if(!src) return false;
            uint16_t floatsize = binary::read<uint16_t>(src);
            if(!src) return false;

            return marker == FourierLUT::FILE_MARKER && floatsize == sizeof(Float);
        }
    }


    std::vector<Complex> FourierLUT::eval(int r, int g, int b) const
    {
        (void) r;
        (void) g;
        (void) b;
        return {};
    }

    FourierLUT FourierLUT::load_from(std::istream &src)
    {
        if(!validate_header(src)) throw std::invalid_argument("Unsupported file");
        uint16_t step = binary::read<uint16_t>(src);
        uint16_t m = binary::read<uint16_t>(src);
        FourierLUT lut{step, m};
        const unsigned size = lut.size * lut.size * lut.size;
        for(unsigned i = 0; i < size; ++i) {
            lut.data[i] = binary::read_vec<Float>(src);
        }
        return lut;
    }
}