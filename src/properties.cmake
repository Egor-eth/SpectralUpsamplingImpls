set(PROJECT_SOURCES 
    main.cpp
    argparse.cpp
)

add_subdirectory(${SRC}/lib)
add_subdirectory(${SRC}/apps)
add_subdirectory(${SRC}/3rd_party)

set(PROJECT_LIBS
    lib upsamplers
)