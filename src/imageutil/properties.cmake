set(MODULE_NAME imageutil)
set(MODULE_PATH ${SRC}/imageutil)

set(MODULE_SOURCES
	image.cpp
	pixel.cpp
	spectral_image.cpp
)

set(MODULE_LIBS
	glm stb
)