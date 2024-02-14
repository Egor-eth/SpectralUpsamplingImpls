#include "lutworks.h"
#include "serialization/binary.h"
#include "common/util.h"

namespace bin = spec::binary;

void write_header(std::ostream &dst)
{
    bin::write<uint64_t>(dst, spec::LUT::FILE_MARKER);
    bin::write<uint16_t>(dst, sizeof(Float));
}

void write_lut(std::ostream &dst, const LUT &lut)
{  
    unsigned step = lut.get_step();
    bin::write<uint16_t>(dst, step);
    unsigned count = lut.get_size();
    count = count * count * count;
    const vec3 *ptr = lut.get_raw_data();
    for(unsigned i = 0; i < count ; ++i) {
        bin::write_vec<Float>(dst, ptr[i]);
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
        const int main_channel;
        const int step, size;
        const int stable, stable_id;
        const bool force_last;
        int i = 0, j = 0, k = 0;

        LutBuilder(int main_channel, int step, int stable)
            : main_channel{main_channel}, step{step}, size{256 / step + (255 % step != 0)},
              stable{stable}, stable_id{stable / step}, 
              force_last{255 % step != 0}, data{new vec3[size * size * size]} {}

        ~LutBuilder()
        {
            delete[] data;
        }

        LUT build_and_clear()
        {
            vec3 *d = data;
            data = nullptr;
            return LUT(d, step);
        }

        int idx_to_color(int i) const
        {
            return i == size - 1 ? 255 : i * step;
        }

        void set_target(int i, int j, int k);

        int color_to_idx(int c) const
        {
            return c == 255 ? size - 1 : c / step; 
        }

        Float acolorf(int i) const
        {
            return spec::math::smoothstep2(i / static_cast<Float>(size - 1));
        }

        void init_solution(vec3d &solution) const
        {
            if(k == stable_id) {
                std::fill_n(solution.v, 3, 0.0);
            }
            else {
                solution = at(i, j, k < stable_id ? k + 1 : k - 1);
            }
        }

        const vec3 &at(int i1, int j1, int k1) const
        {
            return data[((k1 * size) + i1) * size + j1]; 
        }

        const vec3 &current() const
        {
            return data[((k * size) + i) * size + j]; 
        }

        vec3 &current()
        {
            return data[((k * size) + i) * size + j]; 
        }

        vec3 spaced_color() const;
    private:
        vec3 *data;
    };

    vec3 LutBuilder::spaced_color() const
    {
        Float ac = acolorf(k);
        return fill_vector(main_channel, ac * idx_to_color(i) / 255.0f, ac * idx_to_color(j) / 255.0f, ac);
    }


    /**
     *  ____________________
     * /                  /|
     * |------------------||
     * | 00 |    10       ||
     * |    |             ||
     * |------------------||
     * |    |             ||
     * |    |             ||
     * | 01 |    11       ||
     * |    |             ||
     * ___________________|/
     */

    unsigned color_processed = 0;

    void fill_main(LutBuilder &ctx)
    {
        vec3d solution;

        for(ctx.k = 0; ctx.k < ctx.size; ++ctx.k) {
            //over x, stable
            ctx.j = ctx.stable_id;
            for(ctx.i = ctx.stable_id; ctx.i >= 0; --ctx.i) {
                ctx.init_solution(solution);
                solve_for_rgb_d(ctx.spaced_color(), solution);
                ctx.current() = solution;
                color_processed += 1;
            }

            spec::print_progress(color_processed);

            ctx.j = ctx.stable_id;
            ctx.init_solution(solution);
            for(ctx.i = ctx.stable_id + 1; ctx.i < ctx.size; ++ctx.i) {
                solve_for_rgb_d(ctx.spaced_color(), solution);
                ctx.current() = solution;
                color_processed += 1;
            }
            spec::print_progress(color_processed);

            //over y, stable
            ctx.i = ctx.stable_id;
            for(ctx.j = ctx.stable_id - 1; ctx.j >= 0; --ctx.j) {
                ctx.init_solution(solution);
                solve_for_rgb_d(ctx.spaced_color(), solution);
                ctx.current() = solution;
                color_processed += 1;
            }
            spec::print_progress(color_processed);
            //over y, unstable
            ctx.i = ctx.stable_id;
            ctx.init_solution(solution);
            for(ctx.j = ctx.stable_id + 1; ctx.j <= ctx.size; ++ctx.j) {
                solve_for_rgb_d(ctx.spaced_color(), solution);
                ctx.current() = solution;
                color_processed += 1;
            }
            spec::print_progress(color_processed);
        }
    }

    void fill_00(LutBuilder &ctx, int target)
    {
        vec3d solution;

        //over x
        ctx.j = target;
        for(ctx.i = target; ctx.i >= 0; --ctx.i) {
            const int dy = 1;
            const int dx = target - ctx.i + 1; 

            const Float wx = dy / static_cast<Float>(dx + dy);
            const Float wy = dx / static_cast<Float>(dx + dy);

            solution = ctx.at(ctx.i, target + 1, ctx.k) * wy + ctx.at(target + 1, target, ctx.k) * wx;

            solve_for_rgb_d(ctx.spaced_color(), solution);
            ctx.current() = solution;
            color_processed += 1;
        }
        spec::print_progress(color_processed);
        //over y
        ctx.i = target;
        for(ctx.j = target - 1; ctx.j >= 0; --ctx.j) {
            const int dy = 1;
            const int dx = target - ctx.j + 1; 

            const Float wy = dx / static_cast<Float>(dx + dy);
            const Float wx = dy / static_cast<Float>(dx + dy);

            solution = ctx.at(target + 1, ctx.j, ctx.k) * wx + ctx.at(target, target + 1, ctx.k) * wy;

            solve_for_rgb_d(ctx.spaced_color(), solution);
            ctx.current() = solution;
            color_processed += 1;
        }
        spec::print_progress(color_processed);
    }

    void fill_11(LutBuilder &ctx, int target)
    {
        vec3d solution;

        //over x
        ctx.j = target;
        for(ctx.i = target; ctx.i < ctx.size; ++ctx.i) {
            const int dy = 1;
            const int dx = ctx.i - target + 1; 

            const Float wx = dy / static_cast<Float>(dx + dy);
            const Float wy = dx / static_cast<Float>(dx + dy);

            solution = ctx.at(ctx.i, target - 1, ctx.k) * wy + ctx.at(target - 1, target, ctx.k) * wx;

            solve_for_rgb_d(ctx.spaced_color(), solution);
            ctx.current() = solution;
            color_processed += 1;
        }
        spec::print_progress(color_processed);
        //over y
        ctx.i = target;
        for(ctx.j = target + 1; ctx.j < ctx.size; ++ctx.j) {
            const int dy = 1;
            const int dx = ctx.j - target + 1; 

            const Float wy = dx / static_cast<Float>(dx + dy);
            const Float wx = dy / static_cast<Float>(dx + dy);

            solution = ctx.at(target - 1, ctx.j, ctx.k) * wx + ctx.at(target, target - 1, ctx.k) * wy;

            solve_for_rgb_d(ctx.spaced_color(), solution);
            ctx.current() = solution;
            color_processed += 1;
        }
        spec::print_progress(color_processed);
    }

    void fill_01(LutBuilder &ctx, int target_x, int target_y)
    {
        vec3d solution;

        //over x
        ctx.j = target_y;
        for(ctx.i = target_x; ctx.i >= 0; --ctx.i) {
            const int dy = 1;
            const int dx = target_x - ctx.i + 1; 

            const Float wx = dy / static_cast<Float>(dx + dy);
            const Float wy = dx / static_cast<Float>(dx + dy);

            solution = ctx.at(ctx.i, target_y - 1, ctx.k) * wy + ctx.at(target_x + 1, target_y, ctx.k) * wx;

            solve_for_rgb_d(ctx.spaced_color(), solution);
            ctx.current() = solution;
            color_processed += 1;
        }
        spec::print_progress(color_processed);
        //over y
        ctx.i = target_x;
        for(ctx.j = target_y + 1; ctx.j < ctx.size; ++ctx.j) {
            const int dy = 1;
            const int dx = ctx.j - target_y + 1; 

            const Float wy = dx / static_cast<Float>(dx + dy);
            const Float wx = dy / static_cast<Float>(dx + dy);

            solution = ctx.at(target_x + 1, ctx.j, ctx.k) * wx + ctx.at(target_x, target_y - 1, ctx.k) * wy;

            solve_for_rgb_d(ctx.spaced_color(), solution);
            ctx.current() = solution;
            color_processed += 1;
        }
        spec::print_progress(color_processed);
    }

    void fill_10(LutBuilder &ctx, int target_x, int target_y)
    {
        vec3d solution;

        //over x
        ctx.j = target_y;
        for(ctx.i = target_x; ctx.i < ctx.size; ++ctx.i) {
            const int dy = 1;
            const int dx = ctx.i - target_x + 1; 

            const Float wx = dy / static_cast<Float>(dx + dy);
            const Float wy = dx / static_cast<Float>(dx + dy);

            solution = ctx.at(ctx.i, target_y + 1, ctx.k) * wy + ctx.at(target_x - 1, target_y, ctx.k) * wx;

            solve_for_rgb_d(ctx.spaced_color(), solution);
            ctx.current() = solution;
            color_processed += 1;
        }
        spec::print_progress(color_processed);
        //over y
        ctx.i = target_x;
        for(ctx.j = target_y - 1; ctx.j >= 0; --ctx.j) {
            const int dy = 1;
            const int dx = target_y - ctx.j + 1; 

            const Float wy = dx / static_cast<Float>(dx + dy);
            const Float wx = dy / static_cast<Float>(dx + dy);

            solution = ctx.at(target_x - 1, ctx.j, ctx.k) * wx + ctx.at(target_x, target_y + 1, ctx.k) * wy;

            solve_for_rgb_d(ctx.spaced_color(), solution);
            ctx.current() = solution;
            color_processed += 1;
        }
        spec::print_progress(color_processed);
    }

    void fill_stupid(LutBuilder &ctx)
    {
        vec3d solution;
        for(ctx.i = 0; ctx.i < ctx.size; ++ctx.i) {
            for(ctx.j = 0; ctx.j < ctx.size; ++ctx.j) {
                ctx.init_solution(solution);

                solve_for_rgb_d(ctx.spaced_color(), solution);
                ctx.current() = solution;
                color_processed += 1;
            }
            spec::print_progress(color_processed);
        }

    }

}

#include <iostream>

LUT generate_lut(int zeroed_idx, int step, int stable_val)
{
    LutBuilder ctx{zeroed_idx, step, stable_val};

    color_processed = 0u;

    spec::init_progress_bar(ctx.size * ctx.size * ctx.size);

    for(ctx.k = ctx.stable_id; ctx.k >= 0; --ctx.k) {
        fill_stupid(ctx);
    }
    for(ctx.k = ctx.stable_id + 1; ctx.k < ctx.size; ++ctx.k) {
        fill_stupid(ctx);
    }

    /*
    fill_main(ctx);

    for(ctx.k = ctx.stable_id; ctx.k >= 0; --ctx.k) {
        for(int i = ctx.stable_id - 1; i >= 0; --i) {
            fill_00(ctx, i);
        }
       // std::cout << "00 - " << ctx.k << std::endl;
        for(int i = ctx.stable_id + 1; i < ctx.size; ++i) {
            fill_11(ctx, i);
        }
      //  std::cout << "11 - " << ctx.k << std::endl;
        for(int i = ctx.stable_id - 1, j = ctx.stable_id + 1; i >= 0 && j < ctx.size; --i, ++j) {
            fill_01(ctx, i, j);
      //      std::cout << "01 - " << ctx.i << " " << j << std::endl;
        }
        for(int i = ctx.stable_id + 1, j = ctx.stable_id - 1; j >= 0 && i < ctx.size; ++i, --j) {
            fill_10(ctx, i, j);
        }
     //   std::cout << "10 - " << ctx.k << std::endl;
    }

    for(ctx.k = ctx.stable_id + 1; ctx.k < ctx.size; ++ctx.k) {
        for(int i = ctx.stable_id - 1; i >= 0; --i) {
            fill_00(ctx, i);
        }
       // std::cout << "00 - " << ctx.k << std::endl;
        for(int i = ctx.stable_id + 1; i < ctx.size; ++i) {
            fill_11(ctx, i);
        }
      //  std::cout << "11 - " << ctx.k << std::endl;
        for(int i = ctx.stable_id - 1, j = ctx.stable_id + 1; i >= 0 && j < ctx.size; --i, ++j) {
            fill_01(ctx, i, j);
      //      std::cout << "01 - " << ctx.i << " " << j << std::endl;
        }
        for(int i = ctx.stable_id + 1, j = ctx.stable_id - 1; j >= 0 && i < ctx.size; ++i, --j) {
            fill_10(ctx, i, j);
        }
     //   std::cout << "10 - " << ctx.k << std::endl;
    }*/
    spec::finish_progress_bar();
    return ctx.build_and_clear();
}