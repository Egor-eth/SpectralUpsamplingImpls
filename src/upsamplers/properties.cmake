set(MODULE_NAME upsamplers)
set(MODULE_PATH ${SRC}/upsamplers)

set(MODULE_SOURCES
	naive_upsampler.cpp
)

set(MODULE_LIBS
	glm imageutil progress_bar
)