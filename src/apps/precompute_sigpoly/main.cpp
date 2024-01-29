#include <iostream>
#include <ceres/ceres.h>
#include <glog/logging.h>
#include "spec/conversions.h"

struct CostFunctor {
    template<typename T>
    bool operator()(const T *const x, T *residual)
    {
        
        return true;
    }
};

int main(int argc, char **argv)
{
    google::InitGoogleLogging(argv[0]);

    
    return 0;
} 
