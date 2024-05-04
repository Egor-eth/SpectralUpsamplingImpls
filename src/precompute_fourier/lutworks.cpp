#include "lutworks.h"
#include <internal/serialization/binary.h>
#include <internal/common/util.h>
#include <spec/conversions.h>
#include <vector>
#include <limits>
#include <algorithm>

#include <iostream>

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

    const auto &pvals = lut.get_power_vals();
    bin::write<uint16_t>(dst, pvals.size());
    for(unsigned i = 0; i < pvals.size(); ++i) {
        bin::write<Float>(dst, pvals[i]);
    }

    unsigned count = lut.get_size();
    count = pvals.size() * count * count * count * (lut.get_m() + 1);
    const Float *ptr = lut.get_raw_data();
    for(unsigned i = 0; i < count ; ++i) {
        bin::write<Float>(dst, ptr[i]);
    }
}
constexpr int DEFAULT_P_ID = 3;


namespace {

    class LutBuilder
    {
    public:

        const unsigned step, size;
        const bool force_last;
        const int m;
        long i = 0, j = 0, k = 0, n = 0;
        const std::vector<Float> power_values{1.0f, 5.0f, 10.0f, 20.0f, 50.0f, 100.0f};
        const unsigned p_size = power_values.size();
        const std::vector<Float> &dataset_wavelenghts;

        LutBuilder(int m, unsigned step, const std::vector<Float> &seeds_moments, std::vector<vec3ui> &&seeds_rgb,
                 const std::vector<Float> &wavelenghts, const std::vector<std::vector<Float>> &dataset)
            : step{step}, size{256u / step + (255u % step != 0u)},
              force_last{255u % step != 0u}, m{m}, dataset_wavelenghts(wavelenghts),
              seeds(std::move(seeds_rgb)),
              data(p_size * size * size * size * (m + 1), 0.0f), dataset(dataset)
        {
            for(unsigned k = 0; k < seeds.size(); ++k) {
                Float *out_ptr = at(seeds[k], DEFAULT_P_ID);
                const Float *in_ptr = seeds_moments.data() + (m + 1) * k;
                std::copy(in_ptr, in_ptr + m + 1, out_ptr);
            }
        }

        FourierLUT build_and_clear()
        {
            return FourierLUT(std::move(data), power_values, step, m);
        }

        unsigned idx_to_color(unsigned i) const
        {
            return i == size - 1 ? 255 : i * step;
        }

        vec3ui get_target() const
        {
            return {idx_to_color(i), idx_to_color(j), idx_to_color(k)};
        }

        Float target_power() const
        {
            return power_values[n];
        }

        int color_to_idx(int c) const
        {
            return c >= 255 ? size - 1 : c / step; 
        }

        bool init_solution(std::vector<double> &solution, std::vector<Float> &values, unsigned k) const;

        const Float *at(int i1, int j1, int k1, int n1) const
        {
            return data.data() + (((((n1 * size) + i1) * size) + j1) * size + k1) * (m + 1); 
        }

        Float *at(int i1, int j1, int k1, int n1) 
        {
            return const_cast<Float *>(const_cast<const LutBuilder *>(this)->at(i1, j1, k1, n1));
        }

        const Float *at(const vec3ui &rgb, int n1) const
        {
            return at(color_to_idx(rgb.x), color_to_idx(rgb.y), color_to_idx(rgb.z), n1);
        }

        Float *at(const vec3ui &rgb, int n1) 
        {
            return at(color_to_idx(rgb.x), color_to_idx(rgb.y), color_to_idx(rgb.z), n1);
        }

        const Float *current() const
        {
            return at(i, j, k, n);
        }

        Float *current()
        {
            return at(i, j, k, n);
        }

    private:
        const std::vector<vec3ui> seeds;
        std::vector<Float> data;
        const std::vector<std::vector<Float>> &dataset;
    };

    //res and distances must have size >= k
    bool k_nearest(const std::vector<vec3ui> &coords, vec3ui v, unsigned k, std::vector<unsigned> &res, std::vector<double> &distances)
    {   
        k = k <= coords.size() ? k : coords.size();
        std::fill_n(distances.begin(), k, std::numeric_limits<double>::infinity());
        vec3 vf = rgb2cielab(v.cast<Float>() / 255.0f);
        for(unsigned n = 0; n < coords.size(); ++n) {
            const auto &rgb = coords[n];
            double dist = vec3::distance(rgb2cielab(rgb.cast<Float>() / 255.0f), vf);
            if(dist == 0.0) return false;
            for(unsigned i = 0; i < distances.size(); ++i) {
                if(dist < distances[i]) {
                    distances.insert(distances.begin() + i, dist);
                    res.insert(res.begin() + i, n);
                    break;
                }
            }
        }
        res.resize(k);
        distances.resize(k);
        return true;
    }

    bool LutBuilder::init_solution(std::vector<double> &solution, std::vector<Float> &values, unsigned knearest) const
    {
        if(n == DEFAULT_P_ID) {
            std::vector<unsigned> nearest(knearest);
            std::vector<double> distances(knearest);
            knearest = nearest.size();
            if(k_nearest(seeds, get_target(), knearest, nearest, distances)) {

                solution.resize(m + 1);
                std::fill_n(solution.begin(), m + 1, 0.0);
                std::fill_n(values.begin(), dataset_wavelenghts.size(), 0.0f);

                double div = 0.0;
                for(unsigned i = 0; i < knearest; ++i) div += 1.0 / distances[i];

                for(unsigned i = 0; i < knearest; ++i) {
                    const double mul = (1.0 / distances[i]) / div;
                    const auto &rgb = seeds[nearest[i]];
                    const Float *target = at(rgb, DEFAULT_P_ID);
                    for(int j = 0; j <= m; ++j) {
                        solution[j] += target[j] * mul;
                    }
                    const std::vector<Float> &target_values = dataset[nearest[i]];
                    for(unsigned j = 0; j < target_values.size(); ++j) {
                        values[j] += target_values[j] * mul;
                    }
                }
                return true;
            }
            return false;
        }
        else {
            int next = n < DEFAULT_P_ID ? n + 1 : n - 1;
            const Float *target = at(i, j, k, next);
            solution.resize(m + 1);

            const Float mul = power_values[n] / power_values[next];
            for(int i = 0; i <= m; ++i) {
                solution[i] = target[i] * mul;
            }

            return true;
        }
    }

    unsigned color_processed = 0;

    void fill_layer(LutBuilder &ctx, unsigned knearest)
    {
        std::vector<double> solution(ctx.m + 1);
        std::vector<Float> values(ctx.dataset_wavelenghts.size());

        for(ctx.i = 0; ctx.i < ctx.size; ++ctx.i) {
            for(ctx.j = 0; ctx.j < ctx.size; ++ctx.j) {
                for(ctx.k = 0; ctx.k < ctx.size; ++ctx.k) {
                    if(ctx.init_solution(solution, values, knearest)) {
                    //TODO
                        solve_for_rgb(ctx.get_target().cast<Float>() / 255.0f, ctx.target_power(), solution, ctx.dataset_wavelenghts, values);
                        std::copy(solution.begin(), solution.end(), ctx.current());
                        color_processed += 1;
                    }
                    spec::print_progress(color_processed);
                }
            }

        }
    }

    void fill(LutBuilder &ctx, unsigned knearest)
    {
        for(ctx.n = DEFAULT_P_ID; ctx.n < ctx.p_size; ++ctx.n) {
            fill_layer(ctx, knearest);
        }
        for(ctx.n = DEFAULT_P_ID - 1; ctx.n >= 0; --ctx.n) {
            fill_layer(ctx, knearest);
        }
    }

    void prepare_seeds(const std::vector<Float> &in_wavelenghts, const std::vector<std::vector<Float>> &in_seeds, std::vector<vec3ui> &rgbs, Float power, std::vector<Float> &out_moments, int step)
    {   
        init_progress_bar(in_seeds.size());
        color_processed = 0u;

        auto phases = math::wl_to_phases(in_wavelenghts);
        for(unsigned i = 0; i < rgbs.size(); ++i) {
            vec3ui rgb = rgbs[i];
            rgb.x = rgb.x == 255 ? 255 : (rgb.x / step) * step;
            rgb.y = rgb.y == 255 ? 255 : (rgb.y / step) * step;
            rgb.z = rgb.z == 255 ? 255 : (rgb.z / step) * step;

            if(rgb != rgbs[i]) {
                //std::cout << rgb << " <- " << rgbs[i] << std::endl;
                std::vector<double> res = adjust_and_compute_moments(rgb.cast<Float>() / 255.0f, power, in_wavelenghts, in_seeds[i]);
                res.resize(M + 1);
                out_moments.insert(out_moments.end(), res.begin(), res.end());
                rgbs[i] = rgb;
            }
            else {
                std::vector<Float> res = math::real_fourier_moments_of(phases, in_seeds[i], M + 1);
                out_moments.insert(out_moments.end(), res.begin(), res.end());
            }
            print_progress(++color_processed);
        }
        finish_progress_bar();
    }

}

FourierLUT generate_lut(const std::vector<Float> &wavelenghts, const std::vector<std::vector<Float>> &seeds, std::vector<vec3ui> rgbs, unsigned step, unsigned knearest)
{
    std::vector<Float> seeds_moments;

    prepare_seeds(wavelenghts, seeds, rgbs, 20.0f, seeds_moments, step);
    
    color_processed = 0u;

    LutBuilder ctx{M, step, seeds_moments, std::move(rgbs), wavelenghts, seeds};
    init_progress_bar(ctx.p_size * ctx.size * ctx.size * ctx.size - seeds.size(), 256);

    fill(ctx, knearest);

    finish_progress_bar();
    return ctx.build_and_clear();
}