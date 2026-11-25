function(eject_add_module name)
    cmake_parse_arguments(PARSE_ARGV 1 argument "SHARED" "" "DEPENDS")

    set(target "Eject.${name}")
    set(foreignPlatforms Windows Linux macOS)
    list(REMOVE_ITEM foreignPlatforms "${EJECT_PLATFORM}")
    list(JOIN foreignPlatforms "|" foreignPlatformPattern)

    file(GLOB_RECURSE sources CONFIGURE_DEPENDS RELATIVE "${PROJECT_SOURCE_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.mm")
    list(FILTER sources EXCLUDE REGEX "(^|/)(Eject\\.)?Platform/(${foreignPlatformPattern})/")

    set(translationUnits ${sources})
    list(FILTER translationUnits INCLUDE REGEX "\\.(cpp|mm)$")

    list(TRANSFORM sources PREPEND "${PROJECT_SOURCE_DIR}/")
    list(TRANSFORM argument_DEPENDS PREPEND "Eject.")

    if(NOT translationUnits)
        add_library(${target} INTERFACE ${sources})
        target_include_directories(${target} INTERFACE "${PROJECT_SOURCE_DIR}")
        target_link_libraries(${target} INTERFACE ${argument_DEPENDS})
    else()
        if(argument_SHARED)
            add_library(${target} SHARED ${sources})
            set_target_properties(${target} PROPERTIES PREFIX "")
        else()
            add_library(${target} STATIC ${sources})
        endif()
        target_include_directories(${target} PUBLIC "${PROJECT_SOURCE_DIR}")
        target_link_libraries(${target} PUBLIC ${argument_DEPENDS} PRIVATE Eject.BuildOptions)
    endif()
endfunction()