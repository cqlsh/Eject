cmake_minimum_required(VERSION 3.28)

project(Eject VERSION 0.1.0 LANGUAGES CXX)

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(EJECT_PLATFORM "macOS")
else()
    set(EJECT_PLATFORM "${CMAKE_SYSTEM_NAME}")
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/CMake/${EJECT_PLATFORM}/CompilerOptions.cmake")
    message(FATAL_ERROR "Eject does not support ${CMAKE_SYSTEM_NAME}.")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Lib")

include("CMake/${EJECT_PLATFORM}/CompilerOptions.cmake")
include("CMake/EjectModule.cmake")

file(GLOB moduleLists CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/Eject.*/CMakeLists.txt")
foreach(moduleList IN LISTS moduleLists)
    cmake_path(GET moduleList PARENT_PATH moduleDirectory)
    add_subdirectory("${moduleDirectory}")
endforeach()