# DEI_FOUND
# DEI_INCLUDE_DIR
# DEI_LIBRARIES

include(${DEI_DIR}/DivideEtImperaConfigVersion.cmake)
include(${DEI_DIR}/DivideEtImperaConfig.cmake)
include(${DEI_DIR}/DivideEtImperaTargets.cmake)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(DivideEtImpera REQUIRED_VARS DEI_LIBRARY_DIR DEI_LIBRARIES DEI_INCLUDE_DIR VERSION_VAR PACKAGE_VERSION)
mark_as_advanced(DEI_INCLUDE_DIR DEI_LIBRARY_DIR)

if(DivideEtImpera_FOUND)
    set(DEI_FOUND TRUE)
endif()
