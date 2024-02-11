#include "lutworks.h"
#include "serialization/binary.h"
#include "common/util.h"

namespace bin = spec::binary;

constexpr uint64_t MARKER = 0xfafa0000ab0ba000;

void write_header(std::ostream &dst)
{
    bin::write<uint64_t>(dst, MARKER);
    bin::write<uint16_t>(dst, sizeof(Float));
}

bool validate_header(std::istream &src)
{
    uint64_t marker = bin::read<uint64_t>(src);
    if(!src) return false;
    uint16_t floatsize = bin::read<uint16_t>(src);
    if(!src) return false;

    return marker == MARKER && floatsize == sizeof(Float);
}

void write_lut(std::ostream &dst, const LUT &lut)
{  
    bin::write<uint16_t>(dst, lut.get_size());
    for(unsigned i = 0; i < lut.get_size(); ++i) {
        for(unsigned j = 0; j < lut.get_size(); ++j) {
            bin::write_vec(dst, lut.at(i, j));
        }
    }
}



namespace {

    inline vec3 fill_vector(int zeroed_idx, Float a, Float b)
    {
        switch(zeroed_idx) {
        case 0:
            return {0, a, b};
        case 1:
            return {b, 0, a};
        case 2:
            return {a, b, 0};
        default:
            return {};
        }
    }

    inline void _init_x(const LUT &lut, vec3d &solution, int i, int j, bool backwards)
    {
        const int s = backwards ? 1 : -1;
        const vec3 &target = (i > j) ^ backwards ? lut.at(i + s, j) : lut.at(i, j + s);
        solution = target;
    }

    /**
     * -------------------
     * | 00 |    10       |
     * |    |             |
     * |------------------|
     * |    |             |
     * |    |             |
     * | 01 |    11       |
     * |    |             |
     * -------------------
     */

    unsigned color_count = 1;
    unsigned color_processed = 0;

    void fill_main(LUT &lut, int stable, int step, int zeroed_idx)
    {
        vec3d solution;

        Float a, b = stable / 255.0f;

        //over x, stable
        for(int i = stable; i >= 0; i -= step) {
            std::fill_n(solution.v, 3, 0.0);
            a = i / 255.0f;
            solve_for_rgb_d(fill_vector(zeroed_idx, a, b), solution);
            lut.at(i / step, stable / step) = solution;
            color_processed += 1;
        }

        spec::print_progress(color_processed / static_cast<float>(color_count));

        //over x, unstable
        solution = lut.at(stable / step, stable / step);
        for(int i = stable + step; i <= 255; i += step) {
            a = i / 255.0f;
            solve_for_rgb_d(fill_vector(zeroed_idx, a, b), solution);
            lut.at(i / step, stable / step) = solution;
            color_processed += 1;
        }
        spec::print_progress(color_processed / static_cast<float>(color_count));

        //over y, stable
        a = stable / 255.0f;
        for(int i = stable - step; i >= 0; i -= step) {
            std::fill_n(solution.v, 3, 0.0);
            b = i / 255.0f;
            solve_for_rgb_d(fill_vector(zeroed_idx, a, b), solution);
            lut.at(stable / step, i / step) = solution;
            color_processed += 1;
        }
        spec::print_progress(color_processed / static_cast<float>(color_count));
        //over y, unstable
        solution = lut.at(stable, stable);
        for(int i = stable + step; i <= 255; i += step) {
            b = i / 255.0f;
            solve_for_rgb_d(fill_vector(zeroed_idx, a, b), solution);
            lut.at(stable / step, i / step) = solution;
            color_processed += 1;
        }
        spec::print_progress(color_processed / static_cast<float>(color_count));
    }

    void fill_00(LUT &lut, int target, int step, int zeroed_idx)
    {
        vec3d solution;

        Float a, b = target / 255.0f;

        //over x
        for(int i = target; i >= 0; i -= step) {
            const int dy = step;
            const int dx = target - i + step; 

            const Float wx = dy / static_cast<Float>(dx + dy);
            const Float wy = dx / static_cast<Float>(dx + dy);

            solution = lut.at(i / step, target / step + 1) * wy + lut.at(target / step + 1, target / step) * wx;

            a = i / 255.0f;
            solve_for_rgb_d(fill_vector(zeroed_idx, a, b), solution);
            lut.at(i / step, target / step) = solution;
            color_processed += 1;
        }
        spec::print_progress(color_processed / static_cast<float>(color_count));
        //over y
        a = target / 255.0f;
        for(int i = target - step; i >= 0; i -= step) {
            const int dx = step;
            const int dy = target - i + step; 

            const Float wy = dx / static_cast<Float>(dx + dy);
            const Float wx = dy / static_cast<Float>(dx + dy);

            solution = lut.at(target / step + 1, i / step) * wx + lut.at(target / step, target / step + 1) * wy;

            b = i / 255.0f;
            solve_for_rgb_d(fill_vector(zeroed_idx, a, b), solution);
            lut.at(target / step, i / step) = solution;
            color_processed += 1;
        }
        spec::print_progress(color_processed / static_cast<float>(color_count));
    }

    void fill_11(LUT &lut, int target, int step, int zeroed_idx)
    {
        vec3d solution;

        Float a, b = target / 255.0f;

        //over x
        for(int i = target; i <= 255; i += step) {
            const int dy = step;
            const int dx = i - target + step; 

            const Float wx = dy / static_cast<Float>(dx + dy);
            const Float wy = dx / static_cast<Float>(dx + dy);

            solution = lut.at(i / step, target / step - 1) * wy + lut.at(target / step - 1, target / step) * wx;

            a = i / 255.0f;
            solve_for_rgb_d(fill_vector(zeroed_idx, a, b), solution);
            lut.at(i / step, target / step) = solution;
            color_processed += 1;
        }
        spec::print_progress(color_processed / static_cast<float>(color_count));
        //over y
        a = target / 255.0f;
        for(int i = target + step; i <= 255; i += step) {
            const int dx = step;
            const int dy = i - target + step; 

            const Float wy = dx / static_cast<Float>(dx + dy);
            const Float wx = dy / static_cast<Float>(dx + dy);

            solution = lut.at(target / step - 1, i / step) * wx + lut.at(target / step, target / step - 1) * wy;

            b = i / 255.0f;
            solve_for_rgb_d(fill_vector(zeroed_idx, a, b), solution);
            lut.at(target / step, i / step) = solution;
            color_processed += 1;
        }
        spec::print_progress(color_processed / static_cast<float>(color_count));
    }

    void fill_01(LUT &lut, int target_x, int target_y, int step, int zeroed_idx)
    {
        vec3d solution;

        Float a, b = target_y / 255.0f;

        //over x
        for(int i = target_x; i >= 0; i -= step) {
            const int dy = step;
            const int dx = target_x - i + step; 

            const Float wx = dy / static_cast<Float>(dx + dy);
            const Float wy = dx / static_cast<Float>(dx + dy);

            solution = lut.at(i / step, target_y / step - 1) * wy + lut.at(target_x / step + 1, target_y / step) * wx;

            a = i / 255.0f;
            solve_for_rgb_d(fill_vector(zeroed_idx, a, b), solution);
            lut.at(i / step, target_y / step) = solution;
            color_processed += 1;
        }
        spec::print_progress(color_processed / static_cast<float>(color_count));

        //over y
        a = target_x / 255.0f;
        for(int i = target_y + step; i <= 255; i += step) {
            const int dx = step;
            const int dy = i - target_y + step; 

            const Float wy = dx / static_cast<Float>(dx + dy);
            const Float wx = dy / static_cast<Float>(dx + dy);

            solution = lut.at(target_x / step + 1, i / step) * wx + lut.at(target_x / step, target_y / step - 1) * wy;

            b = i / 255.0f;
            solve_for_rgb_d(fill_vector(zeroed_idx, a, b), solution);
            lut.at(target_x / step, i / step) = solution;
            color_processed += 1;
        }
        spec::print_progress(color_processed / static_cast<float>(color_count));
    }

    void fill_10(LUT &lut, int target_x, int target_y, int step, int zeroed_idx)
    {
        vec3d solution;

        Float a, b = target_y / 255.0f;

        //over x
        for(int i = target_x; i <= 255; i += step) {
            const int dy = step;
            const int dx = i - target_x + step; 

            const Float wx = dy / static_cast<Float>(dx + dy);
            const Float wy = dx / static_cast<Float>(dx + dy);

            solution = lut.at(i / step, target_y / step + 1) * wy + lut.at(target_x / step - 1, target_y / step) * wx;

            a = i / 255.0f;
            solve_for_rgb_d(fill_vector(zeroed_idx, a, b), solution);
            lut.at(i / step, target_y / step) = solution;
            color_processed += 1;
        }
        spec::print_progress(color_processed / static_cast<float>(color_count));

        //over y
        a = target_x / 255.0f;
        for(int i = target_y - step; i >= 0; i -= step) {
            const int dx = step;
            const int dy = target_y - i + step; 

            const Float wy = dx / static_cast<Float>(dx + dy);
            const Float wx = dy / static_cast<Float>(dx + dy);

            solution = lut.at(target_x / step - 1, i / step) * wx + lut.at(target_x / step, target_y / step + 1) * wy;

            b = i / 255.0f;
            solve_for_rgb_d(fill_vector(zeroed_idx, a, b), solution);
            lut.at(target_x / step, i / step) = solution;
            color_processed += 1;
        }
        spec::print_progress(color_processed / static_cast<float>(color_count));
    }

}


LUT generate_lut(int zeroed_idx, int step, int stable_val)
{
    const bool force_last_layer = (256 % step) != 0;
    const unsigned lut_size = 256 / step + force_last_layer;

    color_count = lut_size * lut_size;
    color_processed = 0;

    spec::init_progress_bar();

    LUT lut{lut_size};

    fill_main(lut, stable_val, step, zeroed_idx);
    for(int i = stable_val - step; i >= 0; i -= step) {
        fill_00(lut, i, step, zeroed_idx);
    }
    for(int i = stable_val + step; i <= 255; i += step) {
        fill_11(lut, i, step, zeroed_idx);
    }
    for(int i = stable_val - step, j = stable_val + step; i >= 0 && j <= 255; i -= step, j += step) {
        fill_01(lut, i, j, step, zeroed_idx);
    }
    for(int i = stable_val + step, j = stable_val - step; i <= 255 && j >= 0; i += step, j -= step) {
        fill_10(lut, i, j, step, zeroed_idx);
    }   
    spec::finish_progress_bar();

    return lut;
}