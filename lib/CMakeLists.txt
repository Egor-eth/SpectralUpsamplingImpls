enable_language(CXX)
enable_language(C)
set(CMAKE_CXX_STANDARD 17)

add_compile_options(-m64)

find_package(nlohmann_json REQUIRED)

include(properties.cmake)
