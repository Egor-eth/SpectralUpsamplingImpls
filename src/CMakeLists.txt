cmake_minimum_required(VERSION 3.12)

enable_language(CXX)
enable_language(C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-m64 -Wall -Wextra -g")
set(CMAKE_C_FLAGS "-m64 -Wall -Wextra")

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(SRC ${CMAKE_SOURCE_DIR})
set(PROJECT_NAME spectral_upsampling_impls)

find_package(Ceres REQUIRED)
find_package(nlohmann_json REQUIRED)

project(${PROJECT_NAME})

include(${SRC}/includes.cmake)

include(properties.cmake)

add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} 
    ${PROJECT_LIBS}
)