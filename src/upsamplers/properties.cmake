set(MODULE_NAME upsamplers)
set(MODULE_PATH ${SRC}/upsamplers)

set(MODULE_SOURCES
    glassner_naive.cpp
    smits.cpp
    upsamplers.cpp
)

set(MODULE_LIBS
    spectral
)