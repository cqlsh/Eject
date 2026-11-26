set_target_properties(Eject.Shared PROPERTIES CXX_VISIBILITY_PRESET default)
set_property(TARGET Eject.Shared APPEND PROPERTY LINK_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/Exports.map")
target_link_options(Eject.Shared PRIVATE "LINKER:--version-script=${CMAKE_CURRENT_LIST_DIR}/Exports.map")