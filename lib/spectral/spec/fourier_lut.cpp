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


    std::vector<Float> FourierLUT::eval(int r, int g, int b) const
    {
         if(!validate_c(r, g, b)) {
            return {};
        }

        const int r1_id = r / step;
        const int r2_id = safe_int(r1_id + 1, size);
        const int g1_id = g / step;
        const int g2_id = safe_int(g1_id + 1, size);
        const int b1_id = b / step;
        const int b2_id = safe_int(b1_id + 1, size);

        const int r1 = safe_int(r1_id * step, 256);
        const int r2 = r == 255 ? 256 : safe_int(r2_id * step, 256);
        const int g1 = safe_int(g1_id * step, 256);
        const int g2 = g == 255 ? 256 : safe_int(g2_id * step, 256);
        const int b1 = safe_int(b1_id * step, 256);
        const int b2 = b == 255 ? 256 : safe_int(b2_id * step, 256);

        const Float drf = Float(r2 - r1) / 255.0f;
        const Float drf1 = Float(r - r1) / 255.0f;
        const Float drf2 = Float(r2 - r) / 255.0f;

        const Float dgf = Float(g2 - g1) / 255.0f;
        const Float dgf1 = Float(g - g1) / 255.0f;
        const Float dgf2 = Float(g2 - g) / 255.0f;

        const Float dbf = Float(b2 - b1) / 255.0f;
        const Float dbf1 = Float(b - b1) / 255.0f;
        const Float dbf2 = Float(b2 - b) / 255.0f;

        Float t = drf * dgf * dbf;
        const Float div = t > 0.0f ? (1.0f / t) : 1.0f;

        std::vector<Float> res(m + 1, 0.0f);

        add(res, r1_id, g1_id, b1_id, drf2 * dgf2 * dbf2 * div);
        add(res, r1_id, g1_id, b2_id, drf2 * dgf2 * dbf1 * div);
        add(res, r1_id, g2_id, b1_id, drf2 * dgf1 * dbf2 * div);
        add(res, r1_id, g2_id, b2_id, drf2 * dgf1 * dbf1 * div);
        add(res, r2_id, g1_id, b1_id, drf1 * dgf2 * dbf2 * div);
        add(res, r2_id, g1_id, b2_id, drf1 * dgf2 * dbf1 * div);
        add(res, r2_id, g2_id, b1_id, drf1 * dgf1 * dbf2 * div);
        add(res, r2_id, g2_id, b2_id, drf1 * dgf1 * dbf1 * div);

        return res;
    }

    FourierLUT FourierLUT::load_from(std::istream &src)
    {
        if(!validate_header(src)) throw std::invalid_argument("Unsupported file");
        uint16_t step = binary::read<uint16_t>(src);
        uint16_t m = binary::read<uint16_t>(src);
        FourierLUT lut{step, m};
        const unsigned size = lut.size * lut.size * lut.size * m;
        for(unsigned i = 0; i < size; ++i) {
            lut.data[i] = binary::read<Float>(src);
        }
        return lut;
    }

    void FourierLUT::add(std::vector<Float> &res, unsigned r, unsigned g, unsigned b, Float mul) const
    {
        unsigned offset = ((r * size + g) * size + b) * m;
        for(unsigned i = 0; i <= m; ++i) {
            res[i] += data[offset + i] * mul;
        }
    }
}