set(MODULE_NAME upsampler)
set(MODULE_PATH ${SRC}/apps/upsampler)

add_subdirectory(upsamplers)

set(MODULE_SOURCES
    main.cpp
    argparse.cpp
)

set(MODULE_LIBS
    lib upsamplers
)