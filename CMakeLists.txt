CMAKE_MINIMUM_REQUIRED(VERSION 3.14)

##################################################################
# OPERATIONAL SYSTEMS CONFIGURATION
##################################################################
set(CMAKE_CXX_STANDARD 17)

if(UNIX)
    set(CMAKE_C_COMPILER mpicc)
    set(CMAKE_CXX_COMPILER mpicxx)
    set(CMAKE_CXX_FLAGS "-m64 -std=c++17 -Wall -Wextra -Wnon-virtual-dtor -pedantic -Wunused-but-set-variable" CACHE STRING "Default compiler flags")

    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Release")
    endif()
    string(TOLOWER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
    set(CMAKE_DEBUG_POSTFIX "_d")

    if(BUILD_SHARED_LIBS)
        set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-fPIC" CACHE STRING "Necessary for shared libraries")
        set(LIBRARY_TYPE "shared")
    else()
        set(LIBRARY_TYPE "static")
    endif()

    set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG" CACHE STRING "Default debug compiler flags")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG" CACHE STRING "Default release compiler flags")
else()
    message(FATAL_ERROR "CMAKE HAS NOT BEEN CONFIGURED FOR THE CURRENT OPERATIONAL SYSTEM\n")
endif()

add_definitions("-DTEST_INPUT_DIRECTORY=\"${CMAKE_SOURCE_DIR}/Zeta/Test/\"")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

##################################################################
# ABOUT THE PROJECT
##################################################################
project(DivideEtImpera)
set(VERSION_MAJOR "0")
set(VERSION_MINOR "0")
set(VERSION_PATCH "1")
set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "$ENV{HOME}/Libraries/dei-${VERSION}/${BUILD_TYPE}/${LIBRARY_TYPE}" CACHE PATH "Install location" FORCE)
endif()

##################################################################
# EXTERNAL LIBRARIES
##################################################################
if(DEFINED ENV{CMAKE_MODULES})
    set(CMAKE_MODULE_PATH $ENV{CMAKE_MODULES})
else()
    set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/Zeta/CMakeModules)
endif()

##############
# BOOST
##############
set(BOOST_VERSION "1.70.0")

if(DEFINED ENV{BOOST_DIR})
    set(BOOST_ROOT $ENV{BOOST_DIR}/${BUILD_TYPE}/${LIBRARY_TYPE})
else()
    set(BOOST_ROOT ${CMAKE_SOURCE_DIR}/Zeta/Setup/mshtocgns-2.0.0/Zeta/Libraries/boost-${BOOST_VERSION}/release)
endif()

set(Components_Boost system filesystem unit_test_framework test_exec_monitor mpi serialization)
set(Boost_USE_MULTITHREADED ON)

find_package(Boost ${BOOST_VERSION} COMPONENTS ${Components_Boost} REQUIRED)
if(Boost_FOUND)
    include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
endif()

##############
# CGNS
##############
set(CGNS_VERSION "3.4.0")

if(DEFINED ENV{CGNS_DIR})
    set(CGNS_DIR $ENV{CGNS_DIR}/${BUILD_TYPE}/${LIBRARY_TYPE})
else()
    set(CGNS_DIR ${CMAKE_SOURCE_DIR}/Zeta/Setup/mshtocgns-2.0.0/Zeta/Libraries/cgns-${CGNS_VERSION}/release)
endif()

find_package(CGNS ${CGNS_VERSION} EXACT COMPONENTS cgns_${LIBRARY_TYPE} REQUIRED)
if(CGNS_FOUND)
    include_directories(${CGNS_INCLUDE_DIR})
endif()

##############
# MSHtoCGNS
##############
set(MSHTOCGNS_VERSION "2.0.0")

if(DEFINED ENV{MSHTOCGNS_DIR})
    set(MSHTOCGNS_DIR $ENV{MSHTOCGNS_DIR}/${BUILD_TYPE}/${LIBRARY_TYPE})
else()
    set(MSHTOCGNS_DIR ${CMAKE_SOURCE_DIR}/Zeta/Libraries/mshtocgns-${MSHTOCGNS_VERSION}/${BUILD_TYPE})
endif()

find_package(MSHtoCGNS ${MSHTOCGNS_VERSION} EXACT REQUIRED)
if(MSHTOCGNS_FOUND)
    include_directories(${MSHTOCGNS_INCLUDE_DIR})
endif()

##############
# METIS
##############
set(METIS_VERSION "5.1.0")

if(DEFINED ENV{METIS_DIR})
    set(METIS_DIR $ENV{METIS_DIR}/${BUILD_TYPE}/${LIBRARY_TYPE})
else()
    set(METIS_DIR ${CMAKE_SOURCE_DIR}/Zeta/Libraries/metis-${METIS_VERSION}/${BUILD_TYPE})
endif()

find_package(METIS ${METIS_VERSION} REQUIRED)
if(METIS_FOUND)
    include_directories(${METIS_INCLUDE_DIRS})
endif()

##############
# MACROS
##############
macro(add_executable _target)
    _add_executable(${_target} ${ARGN})
    target_link_libraries(${_target} ${Boost_LIBRARIES})
    target_link_libraries(${_target} ${CGNS_LIBRARIES})
    target_link_libraries(${_target} ${MSHTOCGNS_LIBRARIES})
    target_link_libraries(${_target} ${METIS_LIBRARIES})
endmacro()

set(TargetsConfig "${PROJECT_NAME}Targets")
set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/include)
set(LIB_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/lib)

macro(add_library _target)
    _add_library(${_target} ${ARGN})
    target_link_libraries(${_target} ${Boost_LIBRARIES})
    target_link_libraries(${_target} ${CGNS_LIBRARIES})
    target_link_libraries(${_target} ${MSHTOCGNS_LIBRARIES})
    target_link_libraries(${_target} ${METIS_LIBRARIES})
    set_target_properties(${_target}  PROPERTIES PREFIX "")
    set_target_properties(${_target}  PROPERTIES VERSION ${VERSION})
    set_property(GLOBAL APPEND PROPERTY GlobalTargetList ${_target})
    install(TARGETS ${PROJECT_NAME} EXPORT ${TargetsConfig} DESTINATION ${LIB_INSTALL_DIR})
endmacro()

##################################################################
# COMPONENTS
##################################################################
enable_testing()

add_subdirectory(MetisInterface)
add_subdirectory(MetisInterfaceTest)
add_test(NAME MetisInterfaceTest COMMAND mpirun -np 2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/MetisInterfaceTest)

add_subdirectory(DomainDivider)
add_subdirectory(DomainDividerTest)
add_test(NAME DomainDividerTest COMMAND mpirun -np 2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/DomainDividerTest)

add_subdirectory(ParallelCgnsInterface)
add_subdirectory(ParallelCgnsInterfaceTest)
add_test(NAME ParallelCgnsInterfaceTest COMMAND mpirun -np 2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ParallelCgnsInterfaceTest)

add_subdirectory(Divider)

##################################################################
# INSTALLATION
##################################################################
set(TemplateConfig "${CMAKE_SOURCE_DIR}/Zeta/CMake/${PROJECT_NAME}Config.cmake.in")
set(ProjectConfig "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake")
set(VersionConfig "${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake")
set(PROJECT_INITIALS "DEI")
get_property(PROJECT_LIBRARIES GLOBAL PROPERTY GlobalTargetList)

include(CMakePackageConfigHelpers)
configure_package_config_file(${TemplateConfig} ${ProjectConfig} INSTALL_DESTINATION ${CMAKE_INSTALL_PREFIX} PATH_VARS PROJECT_INITIALS PROJECT_NAME INCLUDE_INSTALL_DIR LIB_INSTALL_DIR PROJECT_LIBRARIES)

write_basic_package_version_file(${VersionConfig} VERSION ${VERSION} COMPATIBILITY ExactVersion)

install(FILES ${ProjectConfig} ${VersionConfig} DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/DivideEtImpera DESTINATION ${INCLUDE_INSTALL_DIR})
install(EXPORT ${TargetsConfig} DESTINATION ${CMAKE_INSTALL_PREFIX})

##################################################################
# DISPLAY INFORMATION
##################################################################
message("\n")
message(STATUS "Project: ${PROJECT_NAME} ${VERSION}")
message(STATUS "Build type: ${BUILD_TYPE}")
message(STATUS "Library type: ${LIBRARY_TYPE}")

message("\n")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "Compile flags: ${CMAKE_CXX_FLAGS}")
message(STATUS "Debug flags: ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "Release flags: ${CMAKE_CXX_FLAGS_RELEASE}")

message("\n")
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
