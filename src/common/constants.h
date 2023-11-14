#ifndef COMMON_CONSTANTS_H
#define COMMON_CONSTANTS_H
#include <string>
#include "glm/glm.hpp"

constexpr float EPSILON = 1e-5f;

const std::string SPECTREFLOAT_FORMAT = "%.3f";

const glm::mat3x3 RGB_TO_XYZ { //M
    0.4124564f, 0.3575761f, 0.1804375f,
    0.2126729f, 0.7151522f, 0.0721750f,
    0.0193339f, 0.1191920f, 0.9503041f
};

const glm::mat3x3 XYZ_TO_RGB {
    3.2404542f, -1.5371385f, -0.4985314f,
    -0.9692660f, 1.8760108f, 0.0415560f,
    0.0556434f, -0.2040259f, 1.0572252f
};


// Thanks to [http://cvrl.ucl.ac.uk/cmfs.htm] for XYZ curve data
//--------------------------------------------------------------
constexpr int CURVES_WAVELENGHTS[] = {
    390, 395, 400, 405, 410, 415, 
    420, 425, 430, 435, 440, 445, 
    450, 455, 460, 465, 470, 475, 
    480, 485, 490, 495, 500, 505, 
    510, 515, 520, 525, 530, 535, 
    540, 545, 550, 555, 560, 565, 
    570, 575, 580, 585, 590, 595, 
    600, 605, 610, 615, 620, 625, 
    630, 635, 640, 645, 650, 655, 
    660, 665, 670, 675, 680, 685, 
    690, 695, 700, 705, 710, 715, 
    720, 725, 730, 735, 740, 745, 
    750, 755, 760, 765, 770, 775, 
    780, 785, 790, 795, 800, 805, 
    810, 815, 820, 825, 830
};

constexpr float X_CURVE[] = {
    3.769647e-03f, 9.382967e-03f, 2.214302e-02f, 4.742986e-02f, 8.953803e-02f, 1.446214e-01f, 
    2.035729e-01f, 2.488523e-01f, 2.918246e-01f, 3.227087e-01f, 3.482554e-01f, 3.418483e-01f, 
    3.224637e-01f, 2.826646e-01f, 2.485254e-01f, 2.219781e-01f, 1.806905e-01f, 1.291920e-01f, 
    8.182895e-02f, 4.600865e-02f, 2.083981e-02f, 7.097731e-03f, 2.461588e-03f, 3.649178e-03f, 
    1.556989e-02f, 4.315171e-02f, 7.962917e-02f, 1.268468e-01f, 1.818026e-01f, 2.405015e-01f, 
    3.098117e-01f, 3.804244e-01f, 4.494206e-01f, 5.280233e-01f, 6.133784e-01f, 7.016774e-01f, 
    7.967750e-01f, 8.853376e-01f, 9.638388e-01f, 1.051011f,     1.109767f,     1.143620f, 
    1.151033f,     1.134757f,     1.083928f,     1.007344f,     9.142877e-01f, 8.135565e-01f, 
    6.924717e-01f, 5.755410e-01f, 4.731224e-01f, 3.844986e-01f, 2.997374e-01f, 2.277792e-01f, 
    1.707914e-01f, 1.263808e-01f, 9.224597e-02f, 6.639960e-02f, 4.710606e-02f, 3.292138e-02f, 
    2.262306e-02f, 1.575417e-02f, 1.096778e-02f, 7.608750e-03f, 5.214608e-03f, 3.569452e-03f, 
    2.464821e-03f, 1.703876e-03f, 1.186238e-03f, 8.269535e-04f, 5.758303e-04f, 4.058303e-04f, 
    2.856577e-04f, 2.021853e-04f, 1.438270e-04f, 1.024685e-04f, 7.347551e-05f, 5.259870e-05f, 
    3.806114e-05f, 2.758222e-05f, 2.004122e-05f, 1.458792e-05f, 1.068141e-05f, 7.857521e-06f, 
    5.768284e-06f, 4.259166e-06f, 3.167765e-06f, 2.358723e-06f, 1.762465e-06f
};

constexpr float Y_CURVE[] = { 
    4.146161e-04f, 1.059646e-03f, 2.452194e-03f, 4.971717e-03f, 9.079860e-03f, 1.429377e-02f,
    2.027369e-02f, 2.612106e-02f, 3.319038e-02f, 4.157940e-02f, 5.033657e-02f, 5.743393e-02f,
    6.472352e-02f, 7.238339e-02f, 8.514816e-02f, 1.060145e-01f, 1.298957e-01f, 1.535066e-01f, 
    1.788048e-01f, 2.064828e-01f, 2.379160e-01f, 2.850680e-01f, 3.483536e-01f, 4.277595e-01f, 
    5.204972e-01f, 6.206256e-01f, 7.180890e-01f, 7.946448e-01f, 8.575799e-01f, 9.071347e-01f, 
    9.544675e-01f, 9.814106e-01f, 9.890228e-01f, 9.994608e-01f, 9.967737e-01f, 9.902549e-01f, 
    9.732611e-01f, 9.424569e-01f, 8.963613e-01f, 8.587203e-01f, 8.115868e-01f, 7.544785e-01f, 
    6.918553e-01f, 6.270066e-01f, 5.583746e-01f, 4.895950e-01f, 4.229897e-01f, 3.609245e-01f, 
    2.980865e-01f, 2.416902e-01f, 1.943124e-01f, 1.547397e-01f, 1.193120e-01f, 8.979594e-02f, 
    6.671045e-02f, 4.899699e-02f, 3.559982e-02f, 2.554223e-02f, 1.807939e-02f, 1.261573e-02f, 
    8.661284e-03f, 6.027677e-03f, 4.195941e-03f, 2.910864e-03f, 1.995557e-03f, 1.367022e-03f, 
    9.447269e-04f, 6.537050e-04f, 4.555970e-04f, 3.179738e-04f, 2.217445e-04f, 1.565566e-04f, 
    1.103928e-04f, 7.827442e-05f, 5.578862e-05f, 3.981884e-05f, 2.860175e-05f, 2.051259e-05f, 
    1.487243e-05f, 1.080001e-05f, 7.863920e-06f, 5.736935e-06f, 4.211597e-06f, 3.106561e-06f, 
    2.286786e-06f, 1.693147e-06f, 1.262556e-06f, 9.422514e-07f, 7.053860e-07f
};

constexpr float Z_CURVE[] = {
    1.847260e-02f, 4.609784e-02f, 1.096090e-01f, 2.369246e-01f, 4.508369e-01f, 7.378822e-01f, 
    1.051821f,     1.305008f,     1.552826f,     1.748280f,     1.917479f,     1.918437f, 
    1.848545f,     1.664439f,     1.522157,      1.428440f,     1.250610f,     9.991789e-01f, 
    7.552379e-01f, 5.617313e-01f, 4.099313e-01f, 3.105939e-01f, 2.376753e-01f, 1.720018e-01f, 
    1.176796e-01f, 8.283548e-02f, 5.650407e-02f, 3.751912e-02f, 2.438164e-02f, 1.566174e-02f, 
    9.846470e-03f, 6.131421e-03f, 3.790291e-03f, 2.327186e-03f, 1.432128e-03f, 8.822531e-04f, 
    5.452416e-04f, 3.386739e-04f, 2.117772e-04f, 1.335031e-04f, 8.494468e-05f, 5.460706e-05f, 
    3.549661e-05f, 2.334738e-05f, 1.554631e-05f, 1.048387e-05f, 0.0f,          0.0f, 
    0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f, 
    0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f, 
    0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f, 
    0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f, 
    0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f, 
    0.0f,          0.0f,          0.0f,          0.0f,          0.0f,          0.0f, 
    0.0f,          0.0f,          0.0f,          0.0f,          0.0f
};

constexpr size_t CURVES_ARRAY_LEN = sizeof(CURVES_WAVELENGHTS) / sizeof(int); 

//--------------------------------------------------------------

#endif