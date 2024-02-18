set(MODULE_NAME spectral)
set(MODULE_PATH ${SRC}/spectral)

add_subdirectory(common)
add_subdirectory(imageutil)
add_subdirectory(math)
add_subdirectory(serialization)
add_subdirectory(spec)
add_subdirectory(upsample)
#set(MODULE_SOURCES

#)

set(MODULE_LIBS
    common imageutil math spec serialization upsample
)