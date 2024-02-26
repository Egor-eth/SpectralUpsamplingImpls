#include <spec/spectral_util.h>
#include <spec/basic_spectrum.h>
#include <internal/serialization/csv.h>
#include <iostream>
#include <fstream>

#define REPEAT5(X) X, X, X, X, X
#define REPEAT10(X) REPEAT5(X), REPEAT5(X)
#define REPEAT30(X) REPEAT10(X), REPEAT10(X), REPEAT10(X)

using namespace spec;

int main()
{

    std::ifstream file_in{"input/spectrum_m.csv"};
    
    
} 
