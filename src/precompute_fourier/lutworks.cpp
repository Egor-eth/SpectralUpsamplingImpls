#include "lutworks.h"
#include <internal/serialization/binary.h>
#include <internal/common/util.h>
#include <spec/conversions.h>
#include <vector>
#include <limits>
#include <algorithm>

namespace bin = spec::binary;

void write_header(std::ostream &dst)
{
    bin::write<uint64_t>(dst, spec::FourierLUT::FILE_MARKER);
    bin::write<uint16_t>(dst, sizeof(Float));
}

void write_lut(std::ostream &dst, const FourierLUT &lut)
{  
    unsigned step = lut.get_step();
    bin::write<uint16_t>(dst, step);
    bin::write<uint16_t>(dst, lut.get_m());
    unsigned count = lut.get_size();
    count = count * count * count * lut.get_m();
    const Float *ptr = lut.get_raw_data();
    for(unsigned i = 0; i < count ; ++i) {
        bin::write<Float>(dst, ptr[i]);
    }
}



namespace {

    inline vec3 fill_vector(int main_channel, Float a, Float b, Float alpha)
    {
        switch(main_channel) {
        case 0:
            return {alpha, a, b};
        case 1:
            return {b, alpha, a};
        case 2:
            return {a, b, alpha};
        default:
            return {};
        }
    }

    class LutBuilder
    {
    public:
        const int step, size;
        const bool force_last;
        const int m;
        unsigned i = 0, j = 0, k = 0;

        LutBuilder(int m, int step, const std::vector<Float> &seeds_moments, std::vector<vec3ui> &&seeds_rgb)
            : m{m}, step{step}, size{256 / step + (255 % step != 0)},
              force_last{255 % step != 0}, seeds(std::move(seeds_rgb))
              data(size * size * size * m, 0.0f) 
        {
            for(unsigned k = 0; k < seeds.size(); ++k) {
                Float *ptr = at(rgb);
                for(int i = 0u; i <= m; ++i) {
                    ptr[i] = seeds_moments[m * k + i];
                }
            }
        }

        FourierLUT build_and_clear()
        {
            return FourierLUT(std::move(data), step, m);
        }

        int idx_to_color(int i) const
        {
            return i == size - 1 ? 255 : i * step;
        }

        vec3ui get_target() const
        {
            return {idx_to_color(i), idx_to_color(j), idx_to_color(k)};
        }

        int color_to_idx(int c) const
        {
            return c == 255 ? size - 1 : c / step; 
        }

        bool init_solution(std::vector<double> &solution, int k) const;

        const Float *at(int i1, int j1, int k1) const
        {
            return data.data() + ((i1 * size) + j1) * size + k1; 
        }

        const Float *at(const vec3ui &rgb) const
        {
            return at(color_to_idx(rgb.x), color_to_idx(rgb.y), color_to_idx(rgb.z))
        }

        const Float *current() const
        {
            return data.data() + (((i * size) + j) * size + k) * m; 
        }

        Flaot *current()
        {
            return data.data() + (((i * size) + j) * size + k) * m; 
        }

    private:
        const std::vector<vec3ui> seeds;
        const std::vector<Float> data;
    };

    //res and distances must have size >= k
    bool k_nearest(const std::vector<vec3ui> &coords, vec3ui v, int k, std::vector<vec3ui> &res, std::vector<double> &distances)
    {   
        std::fill_n(distances.begin(), k, std::numeric_limits<double>::infinity());
        vec3 vf = v.cast<double>();
        for(const auto &rgb : coords) {
            double dist = vec3::distance(rgb.cast<double>(), vf);
            if(dist == 0.0) return false;
            for(unsigned i = 0; i < distances.size(); ++i) {
                if(dist < distances[i]) {
                    distances.insert(distances.begin() + i, dist);
                    res.insert(res.begin() + i, rgb);
                    break;
                }
            }
        }
        res.resize(k);
        distances.resize(k);
        return true;
    }

    bool LutBuilder::init_solution(std::vector<double> &solution, int knearest) const
    {
        std::vector<vec3ui> nearest(knearest);
        std::vector<double> distances(knearest);
        if(k_nearest(seeds, get_target(), knearest, nearest, distances)) {

            solution.resize(m + 1);
            std::fill_n(solution.begin(), m + 1, 0.0);

            double div = 0.0;
            for(int i = 0; i < knearest; ++i) div += 1.0 / distances[i];

            for(int i = 0; i < knearest; ++i) {
                const double mul = (1.0 / distances[i]) / div;
                const auto &rgb = nearest[i];
                Float *target = at(rgb);
                for(int j = 0; j <= m; ++j) {
                    solution[j] += target[j] * mul;
                }
            }
            return true;
        }
        return false;
    }

    unsigned color_processed = 0;

    void fill(LutBuilder &ctx, int knearest)
    {
        std::vector<double> solution(ctx.m + 1);
        for(ctx.i = 0; ctx.i < ctx.size; ++ctx.i) {
            for(ctx.j = 0; ctx.j < ctx.size; ++ctx.j) {
                for(ctx.k = 0; ctx.k < ctx.size; ++ctx.k) {
                    ctx.init_solution(solution, knearest);
                    //TODO
                    solve_for_rgb_d(ctx.spaced_color(), solution, knearest);
                    ctx.current() = solution;
                    color_processed += 1;
                }
            }
            spec::print_progress(color_processed);

        }

    }

}

#include <iostream>

FourierLUT generate_lut(int m, const std::vector<Float> &seeds, const std::vector<vec3ui> &rgbs, int step = 4);
{
    LutBuilder ctx{m, step, seeds, rgbs};

    color_processed = 0u;

    spec::init_progress_bar(ctx.size * ctx.size * ctx.size);

    fill(ctx);

    spec::finish_progress_bar();
    return ctx.build_and_clear();
}