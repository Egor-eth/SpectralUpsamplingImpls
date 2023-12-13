set(PROJECT_SOURCES 
    main.cpp
    argparse.cpp
)

add_subdirectory(${SRC}/3rd_party/stb)
add_subdirectory(${SRC}/3rd_party/nlohmannjson)
add_subdirectory(${SRC}/lib)
add_subdirectory(${SRC}/upsamplers)
add_subdirectory(${SRC}/apps/precompute_sigpoly)

set(PROJECT_LIBS
    lib upsamplers
)