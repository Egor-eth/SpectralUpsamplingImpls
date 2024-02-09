set(MODULE_NAME serialization)
set(MODULE_PATH ${SRC}/lib/serialization)

set(MODULE_SOURCES
    parsers.cpp
    binary.cpp
)

set(MODULE_LIBS
    math common
)