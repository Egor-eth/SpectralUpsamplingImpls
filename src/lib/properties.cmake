set(MODULE_NAME lib)
set(MODULE_PATH ${SRC}/lib)

add_subdirectory(common)
add_subdirectory(imageutil)
add_subdirectory(math)
add_subdirectory(spec)

set(MODULE_SOURCES
    conversions.cpp
)

set(MODULE_LIBS
    common imageutil math spec 
)