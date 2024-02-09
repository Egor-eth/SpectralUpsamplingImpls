#include "functions.h"
#include <ceres/ceres.h>
#include "spec/conversions.h"
#include "serialization/binary.h"

using namespace ceres;

bool enable_logging = false;

struct CostFunctor {
    const vec3 in;

    CostFunctor(const vec3 &rgb)
        : in(spec::rgb2cielab(rgb)) {}

    template<typename T>
    bool operator()(const T *const x, T *residual) const
    {
        base_vec3<T> result = _xyz2cielab<T>(_sigpoly2xyz<T>(x));

        result -= in.cast<T>();

        residual[0] = result.x;
        residual[1] = result.y;
        residual[2] = result.z;

        return true;
    }
};

vec3d solve_for_rgb(const vec3 &rgb, const vec3d &init)
{
    vec3d x = init;

    if(enable_logging) {
        std::cout << "Using initial x: " << x << std::endl;
    }

    // Build the problem.
    Problem problem;

    // Set up the only cost function (also known as residual). This uses
    // auto-differentiation to obtain the derivative (jacobian).
    CostFunction* cost_function =
      new AutoDiffCostFunction<CostFunctor, 3, 3>(new CostFunctor(rgb));
    problem.AddResidualBlock(cost_function, nullptr, x.v);

    // Run the solver!
    Solver::Options options;
    options.num_threads = 4;
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


std::optional<std::pair<vec3, vec3>> find_stable(const vec3d &init, vec3d &solution, int div, int mdiv, spec::Float epsilon)
{
    const spec::Float divf = div; 
    for(int i = 0; i <= mdiv; ++i) {
        for(int j = 0; j <= mdiv; ++j) {
            for(int k = 0; k <= mdiv; ++k) {
                if(i + j + k == 0) continue;
                vec3 color{i / divf, j / divf, k / divf};
                std::cout << "Using color: " << color << std::endl;

                vec3d result = solve_for_rgb(color, init);
                vec3 downsampled = spec::xyz2rgb(spec::sigpoly2xyz(result.x, result.y, result.z));

                std::cout << "Downsampled version of result: " << downsampled << std::endl;

                if((downsampled - color).abssum() < epsilon) {
                    solution = result;
                    return {{color, downsampled}};
                }
            }
        }
    }

    return {};

}

constexpr uint64_t MARKER = 0xfafa0000ab0ba000;

void write_header(std::ostream &dst)
{
    spec::write<uint64_t>(dst, MARKER);
    spec::write<uint16_t>(dst, sizeof(Float));
}

bool validate_header(std::istream &src)
{
    uint64_t marker = spec::read<uint64_t>(src);
    if(!src) return false;
    uint16_t floatsize = spec::read<uint16_t>(src);
    if(!src) return false;

    return marker == MARKER && floatsize == sizeof(Float);
}