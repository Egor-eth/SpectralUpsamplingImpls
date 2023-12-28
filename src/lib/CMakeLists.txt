include(${SRC}/includes.cmake)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-O0 -g)
else()
    add_compile_options(-O1)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_compile_options(-g)
endif()

include(properties.cmake)

add_library(${MODULE_NAME} INTERFACE)

#target_sources(${MODULE_NAME} PRIVATE 
#    ${MODULE_SOURCES}
#)

target_link_libraries(${MODULE_NAME} INTERFACE
    ${MODULE_LIBS}
)
