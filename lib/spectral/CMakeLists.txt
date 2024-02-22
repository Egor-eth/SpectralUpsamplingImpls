if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-O0 -g)
else()
    add_compile_options(-O1)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_compile_options(-g)
endif()

add_compile_options(-fPIC)

include(properties.cmake)

add_library(${MODULE_NAME} SHARED)

#target_sources(${MODULE_NAME} PRIVATE 
#    ${MODULE_SOURCES}
#)

target_link_libraries(${MODULE_NAME} PUBLIC
    ${MODULE_LIBS}
)
