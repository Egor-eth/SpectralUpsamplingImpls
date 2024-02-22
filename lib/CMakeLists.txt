enable_language(CXX)
enable_language(C)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-Wno-inconsistent-missing-override)
endif()

if(NOT PROJECT_IS_TOP_LEVEL)
    add_compile_options(-Wall -Wextra -Werror -Wcast-qual)
endif()

add_compile_options(-m64)

find_package(nlohmann_json REQUIRED)

include(properties.cmake)
