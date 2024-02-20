set(MODULE_NAME upsample)
set(MODULE_PATH ${SRC}/spectral/upsample)

set(MODULE_SOURCES
    glassner_naive.cpp
    smits.cpp
    upsamplers.cpp
    sigpoly.cpp
)

set(MODULE_LIBS
    spec imageutil spectral-internal
)