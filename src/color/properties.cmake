set(MODULE_NAME color)
set(MODULE_PATH ${SRC}/color)

set(MODULE_SOURCES
    spectral_image.cpp
    spectral_util_1.cpp
    spectral_util_2.cpp
    spectrum.cpp
    conversions.cpp
)

set(MODULE_LIBS
    math stb imageutil nlohmannjson common
)