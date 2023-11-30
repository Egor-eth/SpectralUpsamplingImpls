include_directories("libs/stb")
include_directories("libs")

set(PROJECT_SOURCES 
    main.cpp
    argparse.cpp
)

add_subdirectory(${SRC}/libs/stb)
add_subdirectory(${SRC}/libs/nlohmannjson)
add_subdirectory(${SRC}/upsamplers)
add_subdirectory(${SRC}/imageutil)
add_subdirectory(${SRC}/common)
add_subdirectory(${SRC}/color)
add_subdirectory(${SRC}/math)

set(PROJECT_LIBS
    math upsamplers imageutil color
)