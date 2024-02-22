enable_language(CXX)
enable_language(C)
set(CMAKE_CXX_STANDARD 17)

#Because eigen3 doesn't want to work without it
if(UNIX)
    include_directories(/usr/include/eigen3)
endif()

find_package(Ceres REQUIRED)


if(PROJECT_IS_TOP_LEVEL)
    add_compile_options(-Wall -Wextra -Werror -Wcast-qual)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-O0 -g)
else()
    add_compile_options(-O1)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_compile_options(-g)
endif()




include(properties.cmake)

