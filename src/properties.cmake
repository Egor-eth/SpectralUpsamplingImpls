set(PROJECT_SOURCES 
    main.cpp
    argparse.cpp
)

add_subdirectory(lib)
add_subdirectory(apps)
add_subdirectory(3rd_party)
add_subdirectory(upsamplers)

set(PROJECT_LIBS
    lib upsamplers
)