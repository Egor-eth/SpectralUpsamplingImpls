include(${SRC}/includes.cmake)

include(properties.cmake)

add_executable(${MODULE_NAME}
    ${MODULE_SOURCES}
)

target_link_libraries(${MODULE_NAME}
    ${MODULE_LIBS}
)
