cmake_minimum_required(VERSION 3.12)

enable_language(CXX)
enable_language(C)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-m64)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(SRC ${CMAKE_SOURCE_DIR})
set(PROJECT_NAME spectral_upsampling_impls)


find_package(Ceres REQUIRED)
find_package(nlohmann_json REQUIRED)

project(${PROJECT_NAME})

include(${SRC}/includes.cmake)

include(properties.cmake)
