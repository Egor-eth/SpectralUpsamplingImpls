set(MODULE_NAME color)
set(MODULE_PATH ${SRC}/color)

set(MODULE_SOURCES
    spectral_image.cpp
    spectral_util.cpp
    spectrum.cpp
    conversions.cpp
)

set(MODULE_LIBS
    glm stb imageutil nlohmannjson common
)