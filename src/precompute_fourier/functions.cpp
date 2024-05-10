#include "functions.h"
#include <spec/conversions.h>
#include <spec/basic_spectrum.h>
#include <spec/spectral_util.h>
#include <ceres/ceres.h>
#include <cassert>
#include <algorithm>

using namespace ceres;
using std::abs;

constexpr bool ENABLE_LOG = false;

const Float CIEY_UNIFORM = util::get_cie_y_integral(BasicSpectrum{{360.0f, 1.0f}, {830.0f, 1.0f}});
const vec3 COLOR_POWER{27.4722f, 71.8074f, 7.63813f};

struct CostFunctorEmissionFixer {
    const vec3 in;
    const Float power;
    const std::vector<Float> &wavelenghts;
    const std::vector<Float> phases;
    const std::vector<Float> &values;
    const Float ciey;

    CostFunctorEmissionFixer(const vec3 &rgb, Float power, const std::vector<Float> &wavelenghts, const std::vector<Float> &values)
        : in(rgb2cielab(rgb)), power(power * CIEY_UNIFORM), wavelenghts(wavelenghts), phases(math::wl_to_phases(wavelenghts)), values(values), ciey(_get_cie_y_integral(wavelenghts, values)) {}

    template<typename T>
    bool operator()(const T *const g0, const T *const g, T *residual) const noexcept(true)
    {   
        std::vector<T> vals(M + 1);
        vals[0] = *g0;
        for(int i = 0; i < M; ++i) {
            vals[i + 1] = g[i];
        }
        std::vector<T> result = _mese<T>(phases, vals.data(), M);
        assert(result.size() == phases.size());


        base_vec3<T> xyz = _spectre2xyz0<T>(wavelenghts, result) / T(power);
        base_vec3<T> color = _xyz2cielab<T>(xyz);
        base_vec3<T> color_diff = color - in.cast<T>();

        residual[0] = abs(color_diff.x);
        residual[1] = abs(color_diff.y);
        residual[2] = abs(color_diff.z);

        residual[3] = T(0.0);
        for(unsigned i = 0; i < values.size(); ++i) {
            T a = abs<T>(result[i] - T(values[i]));
            residual[3] += a;
        }

        T p = _get_cie_y_integral(wavelenghts, result);
        residual[4] = abs(p - T(power));
/*
        T min = T(9999999.0);
        T max = T(0.0);
        for(unsigned i = 0; i < wavelenghts.size(); ++i) {
            if(result[i] > max) max = result[i];
            if(result[i] < min) min = result[i]; 
        }

        residual[5] = (max - min);
*/
        return true;
    }
};


std::vector<double> adjust_and_compute_moments(const vec3 &target_rgb, const Float power, const std::vector<Float> &wavelenghts, const std::vector<Float> &values)
{
    std::vector<Float> moments = math::real_fourier_moments_of(math::wl_to_phases(wavelenghts), values, M + 1);
    assert(moments.size() == M + 1);

    std::vector<double> x(moments.begin(), moments.end());

    if(ENABLE_LOG) {
        std::cout << "Fixing for color: " << target_rgb << std::endl;
    }

    // Build the problem.
    Problem problem;

    // Set up the only cost function (also known as residual). This uses
    // auto-differentiation to obtain the derivative (jacobian).
    CostFunction* cost_function =
      new AutoDiffCostFunction<CostFunctorEmissionFixer, 5, 1, M>(new CostFunctorEmissionFixer(target_rgb, power, wavelenghts, values));
    problem.AddResidualBlock(cost_function, nullptr, x.data(), x.data() + 1);

     auto* ordering = new ceres::ParameterBlockOrdering;
    ordering->AddElementToGroup(x.data() + 1, 0);
    ordering->AddElementToGroup(x.data(), 1);

    // Run the solver!
    Solver::Options options;
    options.max_num_iterations = 250;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_progress_to_stdout = ENABLE_LOG;
    options.linear_solver_ordering.reset(ordering);
    Solver::Summary summary;
    Solve(options, &problem, &summary);

    if constexpr(ENABLE_LOG) {
        std::cout << summary.BriefReport() << "\n";
    }

/*
    for(int i = 0; i <= M; ++i) {
        std::cout << x[i] << std::endl;
    }

    std::cout << "S1:" << std::endl;
    for(auto &v : values) {
        std::cout << v << std::endl;;
    }

    auto res = _mese(math::wl_to_phases(wavelenghts), x.data(), M);
    std::cout << "S2:" << std::endl;
    for(auto &v : values) {
        std::cout << v << std::endl;;
    }*/

    return x;
}

struct CostFunctorEmissionConstrained {
    const vec3 in;
    const Float power;
    const std::vector<Float> &wavelenghts;
    const std::vector<Float> phases;
    const std::vector<Float> &values;

    CostFunctorEmissionConstrained(const vec3 &rgb, Float power, const std::vector<Float> &wavelenghts, const std::vector<Float> &values)
        : in(rgb2cielab(rgb)), power(power * CIEY_UNIFORM), wavelenghts(wavelenghts), phases(math::wl_to_phases(wavelenghts)), values(values) {}

    template<typename T>
    bool operator()(const T *const g0, const T *const g, T *residual) const noexcept(true)
    {   
        std::vector<T> vals(M + 1);
        vals[0] = *g0;
        for(int i = 0; i < M; ++i) {
            vals[i + 1] = g[i];
        }
        std::vector<T> result = _mese<T>(phases, vals.data(), M);
        assert(result.size() == phases.size());


        base_vec3<T> xyz = _spectre2xyz0<T>(wavelenghts, result) / T(power);
        base_vec3<T> color = _xyz2cielab<T>(xyz);
        base_vec3<T> color_diff = (color - in.cast<T>()) * COLOR_POWER.cast<T>() * T(25.0);

        if(_xyz2rgb_unsafe(xyz).min() < T(0.0)) return false; 

        residual[0] = abs(color_diff.x);
        residual[1] = abs(color_diff.y);
        residual[2] = abs(color_diff.z);

        
        residual[3] = T(0.0);
        for(unsigned i = 0; i < values.size(); ++i) {
            residual[3] += abs<T>(result[i] - T(values[i]));
        }

        residual[4] = abs(_get_cie_y_integral(wavelenghts, result) - T(power));
   /*

        T min = T(9999999.0);
        T max = T(0.0);
        for(unsigned i = 0; i < wavelenghts.size(); ++i) {
            if(result[i] > max) max = result[i];
            if(result[i] < min) min = result[i]; 
        }

        residual[5] = (max - min);
*/
        //std::cout << residual[0] << " " << residual[1] << " " << residual[2] << " " << residual[3] << " " << std::endl;
        return true;
    }
};

void solve_for_rgb(const vec3 &target_rgb, Float power, std::vector<double> &x, const std::vector<Float> &wavelenghts, const std::vector<Float> &values)
{
    assert(x.size() == M + 1);

    if constexpr(ENABLE_LOG) {
        std::cout << "Solving for color: " << target_rgb << ", power: " << power << std::endl;
    }

    // Build the problem.
    Problem problem;

    // Set up the only cost function (also known as residual). This uses
    // auto-differentiation to obtain the derivative (jacobian).
    CostFunction* cost_function =
      new AutoDiffCostFunction<CostFunctorEmissionConstrained, 5, 1, M>(new CostFunctorEmissionConstrained(target_rgb, power, wavelenghts, values));
    problem.AddResidualBlock(cost_function, nullptr, x.data(), x.data() + 1);

    auto* ordering = new ceres::ParameterBlockOrdering;
    ordering->AddElementToGroup(x.data() + 1, 0);
    ordering->AddElementToGroup(x.data(), 1);

    // Run the solver!
    Solver::Options options;
    options.max_num_iterations = 250;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_progress_to_stdout = ENABLE_LOG;
    options.linear_solver_ordering.reset(ordering);

    Solver::Summary summary;
    Solve(options, &problem, &summary);

    if constexpr(ENABLE_LOG) {
        std::cout << summary.BriefReport() << "\n";
    }
}
