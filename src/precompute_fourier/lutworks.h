#ifndef LUTWORKS_H
#define LUTWORKS_H
#include "functions.h"
#include <spec/fourier_lut.h>
#include <istream>
#include <ostream>
#include <cinttypes>

using namespace spec;

void write_header(std::ostream &dst);
void write_lut(std::ostream &dst, const FourierLUT &lut);

FourierLUT generate_lut(int m, const std::vector<Float> &seeds, const std::vector<vec3ui> &coords, int step = 4);

#endif