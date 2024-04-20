#include "functions.h"
#include <spec/conversions.h>
#include <spec/basic_spectrum.h>
#include <ceres/ceres.h>
#include <cassert>

using namespace ceres;
using std::abs;

constexpr bool ENABLE_LOG = false;

struct CostFunctorEmissionSeedFixer {
    const vec3 in;
    const std::vector<Float> wavelenghts;
    const std::vector<Float> phases;
    const std::vector<Float> values;

    CostFunctorEmissionSeedFixer(const vec3 &rgb, const std::vector<Float> &wavelenghts, const std::vector<Float> &values)
        : in(rgb2cielab(rgb)), wavelenghts(wavelenghts), phases(math::wl_to_phases(wavelenghts)), values(values) {}

    template<typename T>
    bool operator()(const T *const x, T *residual) const noexcept(true)
    {   
        std::vector<T> result = _mese<T>(phases, x, M);
        assert(result.size() == phases.size());

       /* for(auto &v : values) {
            std::cout << v << std::endl;;
        }

        for(auto &v : result) {
            std::cout << v << std::endl;;
        }*/

        base_vec3<T> color = _xyz2cielab<T>(_spectre2xyz<T>(wavelenghts, result));
        base_vec3<T> color_diff = color - in.cast<T>();

        residual[0] = abs(color_diff.x);
        residual[1] = abs(color_diff.y);
        residual[2] = abs(color_diff.z);

        residual[3] = T(0.0);
        for(unsigned i = 0; i < values.size(); ++i) {
            residual[4] += abs<T>(result[i] - T(values[i]));
        }

      //  std::cout << residual[0] << " " << residual[1] << " " << residual[2] << " " << residual[3] << " " << std::endl;

        return true;
    }
};

struct CostFunctorEmissionConstrained {
    const vec3 in;
    const std::vector<Float> &wavelenghts;
    const std::vector<Float> phases;
    const std::vector<Float> &values;

    CostFunctorEmissionConstrained(const vec3 &rgb, const std::vector<Float> &wavelenghts, const std::vector<Float> &values)
        : in(rgb2cielab(rgb)), wavelenghts(wavelenghts), phases(math::wl_to_phases(wavelenghts)), values(values) {}

    template<typename T>
    bool operator()(const T *const x, T *residual) const noexcept(true)
    {   
        std::vector<T> result = _mese<T>(phases, x, M);
        assert(result.size() == phases.size());

        base_vec3<T> color = _xyz2cielab<T>(_spectre2xyz<T>(wavelenghts, result));
        base_vec3<T> color_diff = color - in.cast<T>();

        residual[0] = abs(color_diff.x);
        residual[1] = abs(color_diff.y);
        residual[2] = abs(color_diff.z);

        residual[3] = T(0.0);
        for(unsigned i = 0; i < values.size(); ++i) {
            residual[4] += abs<T>(result[i] - T(values[i]));
        }

        //std::cout << residual[0] << " " << residual[1] << " " << residual[2] << " " << residual[3] << " " << std::endl;
        return true;
    }
};

void solve_for_rgb(const vec3 &target_rgb, std::vector<double> &x, const std::vector<Float> &wavelenghts, const std::vector<Float> &values)
{
    assert(x.size() == M + 1);

    if constexpr(ENABLE_LOG) {
        std::cout << "Solving for color: " << target_rgb << std::endl;
    }

    // Build the problem.
    Problem problem;

    // Set up the only cost function (also known as residual). This uses
    // auto-differentiation to obtain the derivative (jacobian).
    CostFunction* cost_function =
      new AutoDiffCostFunction<CostFunctorEmissionConstrained, 4, M + 1>(new CostFunctorEmissionConstrained(target_rgb, wavelenghts, values));
    problem.AddResidualBlock(cost_function, nullptr, x.data());

    // Run the solver!
    Solver::Options options;
    options.max_num_iterations = 100;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_progress_to_stdout = ENABLE_LOG;
    Solver::Summary summary;
    Solve(options, &problem, &summary);

    if constexpr(ENABLE_LOG) {
        std::cout << summary.BriefReport() << "\n";
    }
}

std::vector<double> adjust_and_compute_moments(const vec3 &target_rgb, const std::vector<Float> &wavelenghts, const std::vector<Float> &values)
{
    std::vector<Float> moments = math::real_fourier_moments_of(math::wl_to_phases(wavelenghts), values, M + 1);
    assert(moments.size() == M + 1);

    std::vector<double> x(moments.begin(), moments.end());

    if(ENABLE_LOG) {
        std::cout << "Solving for color: " << target_rgb << std::endl;
    }

    // Build the problem.
    Problem problem;

    // Set up the only cost function (also known as residual). This uses
    // auto-differentiation to obtain the derivative (jacobian).
    CostFunction* cost_function =
      new AutoDiffCostFunction<CostFunctorEmissionSeedFixer, 4, M + 1>(new CostFunctorEmissionSeedFixer(target_rgb, wavelenghts, values));
    problem.AddResidualBlock(cost_function, nullptr, x.data());

    // Run the solver!
    Solver::Options options;
    options.max_num_iterations = 100;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_progress_to_stdout = ENABLE_LOG;
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