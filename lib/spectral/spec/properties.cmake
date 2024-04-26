set(MODULE_NAME spec)
set(MODULE_PATH ${SRC}/spectral/spec)

set(MODULE_SOURCES
    spectral_util_1.cpp
    spectral_util_2.cpp
    spectral_util_const.cpp
    basic_spectrum.cpp
    sigpoly_spectrum.cpp
    conversions.cpp
    sigpoly_lut.cpp
    fourier_spectrum.cpp
    fourier_lut.cpp
    metrics.cpp
)

set(MODULE_LIBS
    stb nlohmannjson spectral-internal
)