set(MODULE_NAME spectral)
set(MODULE_PATH ${SRC}/lib)

add_subdirectory(common)
add_subdirectory(imageutil)
add_subdirectory(math)
add_subdirectory(spec)

#set(MODULE_SOURCES

#)

set(MODULE_LIBS
    common imageutil math spec
)