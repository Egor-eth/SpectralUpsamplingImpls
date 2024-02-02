#include <iostream>
#include <ceres/ceres.h>
#include <glog/logging.h>
#include <cstring>
#include "functions.h"
#include "common/format.h"
#include "spec/conversions.h"
#include "imageutil/pixel.h"
#include <fstream>
#include "spec/sigpoly_spectrum.h"
#include "common/csv.h"

struct CostFunctor {
    spec::Float in[3];

    CostFunctor(const vec3 &rgb)
        : in() 
        {
            vec3 cielab = spec::xyz2cielab(spec::rgb2xyz(rgb));
            in[0] = cielab.x;
            in[1] = cielab.y;
            in[2] = cielab.z;
        }

    template<typename T>
    bool operator()(const T *const x, T *residual) const
    {
        const base_vec3<T> result = _xyz2cielab<T>(_sigpoly2xyz<T>(x));
        /*
        spec::Float dx = in.x - result.x;
        spec::Float dy = in.y - result.y;
        spec::Float dz = in.z - result.z;

        residual[0] = dx * dx;
        residual[1] = dy * dy;
        residual[2] = dz * dz;
        
        */
        residual[0] = sqrt(base_vec3<T>::distance2(base_vec3<T>(T(in[0]), T(in[1]), T(in[2])), result));

        return true;
    }
};


using namespace ceres;

int main(int argc, char **argv)
{   
    if(argc < 2) {
        return 1;
    }

    std::ifstream initial_x_file{"resources/initial_x.txt"};

    const auto parse_result = spec::csv::load_as_vector<double, double, double>(initial_x_file, ' ')[0];

    const vec3 color = spec::Pixel::from_argb(std::stoi(argv[1], nullptr, 16)).to_vec3();

    google::InitGoogleLogging(argv[0]);

    double x[3]{std::get<0>(parse_result), std::get<1>(parse_result), std::get<2>(parse_result)};

    std::cout << "Using initial x: " << spec::format("%lf %lf %lf", x[0], x[1], x[2]) << std::endl;

    // Build the problem.
    Problem problem;

    // Set up the only cost function (also known as residual). This uses
    // auto-differentiation to obtain the derivative (jacobian).
    CostFunction* cost_function =
      new AutoDiffCostFunction<CostFunctor, 1, 3>(new CostFunctor(color));
    problem.AddResidualBlock(cost_function, nullptr, x);

    // Run the solver!
    Solver::Options options;
    options.max_num_iterations = 100;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_progress_to_stdout = true;
    Solver::Summary summary;
    Solve(options, &problem, &summary);

    std::cout << summary.BriefReport() << "\n";
    std::cout << " -> " << spec::format("%lf %lf %lf", x[0], x[1], x[2]) << "\n";

    vec3 downsampled = spec::xyz2rgb(spec::sigpoly2xyz(x[0], x[1], x[2]));

    std::cout << "Downsampled is " << spec::format("%d %d %d", 
        static_cast<int>(downsampled.x * 255.999f), 
        static_cast<int>(downsampled.y * 255.999f), 
        static_cast<int>(downsampled.z * 255.999f)) << std::endl;


    return 0;
} 
