cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-m64 -Wall -Wextra -g")
set(CMAKE_C_FLAGS "-m64 -Wall -Wextra")

set(SRC ${CMAKE_SOURCE_DIR})
set(PROJECT_NAME spectral_upsampling_impls)


find_package(nlohmann_json REQUIRED)


project(${PROJECT_NAME})

include(properties.cmake)

include(${SRC}/includes.cmake)

add_executable(${PROJECT_NAME}
	${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} 
	${PROJECT_LIBS}
)