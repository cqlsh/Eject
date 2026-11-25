if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(FATAL_ERROR "Eject requires GCC on Linux; found ${CMAKE_CXX_COMPILER_ID}.")
endif()

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

add_compile_options($<$<CONFIG:Release>:-g>)

add_library(Eject.BuildOptions INTERFACE)

target_compile_options(Eject.BuildOptions INTERFACE
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    -Wdouble-promotion
    -Wold-style-cast
    -Wshadow
    -fno-exceptions
    -fno-rtti
    -ffast-math)

target_link_options(Eject.BuildOptions INTERFACE
    -static-libgcc
    -static-libstdc++
    -Wl,--exclude-libs,ALL
    -Wl,--gc-sections
    -Wl,--no-undefined)