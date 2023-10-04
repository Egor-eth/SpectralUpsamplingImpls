include_directories("libs/stb")
include_directories("libs")

set(PROJECT_SOURCES 
	main.cpp
)

add_subdirectory(${SRC}/libs/stb)
add_subdirectory(${SRC}/libs/glm)
add_subdirectory(${SRC}/naive_upsampler)
add_subdirectory(${SRC}/imageutil)

set(${PROJECT_LIBS}
	glm naive_upsampler imageutil
)