cmake_minimum_required(VERSION 3.21)

project(Spectral CXX)

add_library(spectral_proj_compile_options INTERFACE)
target_compile_features(spectral_proj_compile_options INTERFACE cxx_std_17)

option(SPECTRAL_LIB_ONLY "Only compile library")

set(SPECTRAL_PROJECT_ROOT ${PROJECT_SOURCE_DIR})
set(SPECTRAL_LIB_SRC_DIR ${SPECTRAL_PROJECT_ROOT}/lib)
set(SPECTRAL_APPS_SRC_DIR ${SPECTRAL_PROJECT_ROOT}/src)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

include(cmake/includes.cmake)

add_subdirectory(3rd_party)

add_subdirectory(${SPECTRAL_LIB_SRC_DIR})
if(NOT SPECTRAL_LIB_ONLY)
    add_subdirectory(${SPECTRAL_APPS_SRC_DIR})
endif()

