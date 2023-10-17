#ifndef CONSTANTS_H
#define CONSTANTS_H

const int WAVELENGTH_MIN = 380;
const int WAVELENGTH_MAX = 780;
const int WAVELENGTH_STEP = 10;
const int SPECTRE_LENGTH = (WAVELENGTH_MAX - WAVELENGTH_MIN) / WAVELENGTH_STEP; 

const float PI = 3.14159265358f;

const float EPSILON = 1e-5;

#endif