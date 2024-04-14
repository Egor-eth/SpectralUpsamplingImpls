#include "functions.h"
#include <spec/conversions.h>
#include <spec/basic_spectrum.h>
#include <ceres/ceres.h>

using namespace ceres;
using std::abs;

bool enable_logging = false;

struct CostFunctorEmissionConstrained {
    const vec3 in;
    const std::vector<Float> wavelenghts;
    const std::vector<Float> phases;
    const std::vector<Float> values;

    CostFunctorEmissionConstrained(const vec3 &rgb, const std::vector<Float> &wavelenghts, const std::vector<Float> &values)
        : in(rgb2cielab(rgb)), wavelenghts(wavelenghts), phases(math::wl_to_phases(wavelenghts)), values(values) {}

    template<typename T>
    bool operator()(const T *const x, T *residual) const
    {   
        std::vector<T> result = _mese<T>(phases, x, M);

        base_vec3<T> color_diff = (_xyz2cielab<T>(_spectre2xyz<T>(wavelenghts, result)) - in.cast<T>());

        residual[0] = abs(color_diff.x);
        residual[1] = abs(color_diff.y);
        residual[2] = abs(color_diff.z);

        residual[4] = T(0.0);
        for(unsigned i = 0; i < wavelenghts.size(); ++i) {
            residual[4] += abs(result[i] - T(values[i]));
        }

        return true;
    }
};

std::vector<double> solve_for_rgb(const vec3 &rgb, const std::vector<double> &init)
{
    (void) rgb;
    (void) init;
    return {};
}

std::vector<double> adjust_and_compute_moments(const vec3 &target_rgb, const std::vector<Float> &wavelenghts, const std::vector<Float> &values)
{
    std::vector<Float> moments = math::real_fourier_moments_of(math::wl_to_phases(wavelenghts), values, M);

    std::vector<double> x(M + 1);
    for(int i = 0; i <= M; ++i) x[i] = double(moments[i]);

    if(enable_logging) {
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
    options.minimizer_progress_to_stdout = enable_logging;
    Solver::Summary summary;
    Solve(options, &problem, &summary);

    if(enable_logging) {
        std::cout << summary.BriefReport() << "\n";
    }
    return x;
}