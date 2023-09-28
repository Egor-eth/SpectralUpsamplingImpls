cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-m64 -Wall -Wextra -g")


include_directories("libs/stb")
include_directories("libs")


project(spectral_upsampling_impls)

add_executable(spectral_upsampling_impls) 
#target_link_libraries(spectral_upsampling_impls)
