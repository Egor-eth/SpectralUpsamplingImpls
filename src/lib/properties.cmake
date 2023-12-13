set(MODULE_NAME lib)
set(MODULE_PATH ${SRC}/lib)

add_subdirectory(${SRC}/lib/common)
add_subdirectory(${SRC}/lib/imageutil)
add_subdirectory(${SRC}/lib/math)
add_subdirectory(${SRC}/lib/spec)

set(MODULE_SOURCES
    conversions.cpp
)

set(MODULE_LIBS
    common imageutil math spec 
)