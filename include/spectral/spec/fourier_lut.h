#ifndef INCLUDE_SPECTRAL_SPEC_FOURIER_LUT_H
#define INCLUDE_SPECTRAL_SPEC_FOURIER_LUT_H
#include <spectral/internal/math/math.h>
#include <spectral/internal/math/fourier.h>
#include <vector>
#include <cinttypes>
#include <istream>

namespace spec {
    
    class FourierLUT
    {
    public:
        static constexpr uint64_t FILE_MARKER = 0xfafa0000ab0ba001;

        FourierLUT(std::vector<Float> &&data, unsigned step, unsigned m) : step{step}, size{256 / step + (255 % step != 0)}, m{m}, data{std::move(data)} {}

        FourierLUT(unsigned step, unsigned m) : step{step}, size{256 / step + (255 % step != 0)}, m{m}, data(size * size * size * (m + 1)) {}

        FourierLUT() = default;
        
        FourierLUT(const FourierLUT &) = delete;
        FourierLUT &operator=(const FourierLUT &) = delete;

        FourierLUT(FourierLUT &&other) : step{}, size{}, m{}, data{}
        {
            *this = std::move(other);
        }

        FourierLUT &operator=(FourierLUT &&other)
        {
            data = std::move(other.data);
            std::swap(m, other.m);
            std::swap(step, other.step);
            std::swap(size, other.size);
            return *this;
        }

        unsigned get_size() const
        {
            return size;
        }

        unsigned get_step() const
        {
            return step;
        }

        unsigned get_m() const
        {
            return m;
        }

        const Float *get_raw_data() const
        {
            return data.data();
        }

        std::vector<Float> eval(int r, int g, int b) const;

        static FourierLUT load_from(std::istream &src);

    private:
        unsigned step;
        unsigned size;
        unsigned m;
        std::vector<Float> data;

        void add(std::vector<Float> &res, unsigned r, unsigned g, unsigned b, Float mul) const;
    };


}

#endif
