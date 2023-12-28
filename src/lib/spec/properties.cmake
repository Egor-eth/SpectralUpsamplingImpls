set(MODULE_NAME spec)
set(MODULE_PATH ${SRC}/lib/spec)

set(MODULE_SOURCES
    spectral_image.cpp
    spectral_util_1.cpp
    spectral_util_2.cpp
    basic_spectrum.cpp
    sigpoly_spectrum.cpp
    conversions.cpp
)

set(MODULE_LIBS
    stb nlohmannjson imageutil common math
)