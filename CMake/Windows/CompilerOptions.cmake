if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    message(FATAL_ERROR "Eject requires MSVC on Windows; found ${CMAKE_CXX_COMPILER_ID}.")
endif()

string(REPLACE "/EHsc" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT ProgramDatabase)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)

add_library(Eject.BuildOptions INTERFACE)

target_compile_definitions(Eject.BuildOptions INTERFACE
    NOMINMAX
    UNICODE
    WIN32_LEAN_AND_MEAN
    _HAS_EXCEPTIONS=0
    _UNICODE
    _WIN32_WINNT=0x0A00)

target_compile_options(Eject.BuildOptions INTERFACE
    /W4
    /permissive-
    /utf-8
    /volatile:iso
    /Zc:__cplusplus
    /Zc:inline
    /Zc:preprocessor
    /EHs-c-
    /GR-
    /fp:fast
    $<$<CONFIG:Release>:/Gw>)

target_link_options(Eject.BuildOptions INTERFACE
    $<$<CONFIG:Release>:/DEBUG:FULL>
    $<$<CONFIG:Release>:/PDBALTPATH:%_PDB%>
    $<$<CONFIG:Release>:/OPT:REF>
    $<$<CONFIG:Release>:/OPT:ICF>)