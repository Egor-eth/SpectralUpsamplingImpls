include(${SRC}/includes.cmake)

include(properties.cmake)

add_library(${MODULE_NAME})

target_sources(${MODULE_NAME} PRIVATE 
    ${MODULE_SOURCES}
)

target_link_libraries(${MODULE_NAME}
    ${MODULE_LIBS}
)
