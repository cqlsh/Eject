eject_add_module(Shared SHARED DEPENDS Logging)

target_include_directories(Eject.Shared SYSTEM PRIVATE "${PROJECT_SOURCE_DIR}/ThirdParty/ScsSdk/include")

include("Platform/${EJECT_PLATFORM}/Exports.cmake")