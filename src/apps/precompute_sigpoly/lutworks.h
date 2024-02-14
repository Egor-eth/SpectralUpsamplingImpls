#ifndef LUTWORKS_H
#define LUTWORKS_H
#include "functions.h"
#include <istream>
#include <ostream>
#include <cinttypes>
#include "spec/sp_lut.h"

using spec::LUT;

void write_header(std::ostream &dst);
void write_lut(std::ostream &dst, const LUT &lut);

LUT generate_lut(int zeroed_idx, int step = 4, int stable_val = 24);

#endif