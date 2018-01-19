cmake_minimum_required (VERSION 3.5)

# version numbers
set (libidso1070_VERSION_MAJOR 1)
set (libidso1070_VERSION_MINOR 0)

project (libidso1070 VERSION "${libidso1070_VERSION_MAJOR}.${libidso1070_VERSION_MINOR}")

# create config.h
configure_file (
  "config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
)

# create idso1070.pc
configure_file(idso1070.pc.in idso1070.pc @ONLY)

# default GNU install directories
include(GNUInstallDirs)

# set buil to debug
set(CMAKE_BUILD_TYPE Debug)

# set C++ standard to C++11
set (CMAKE_CXX_STANDARD 11)

# add local cmake dir to module path
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)

# add boost as dependency and configure
find_package(Boost 1.58.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

# add binary dir as include directory, to be able to include config.h
include_directories("${PROJECT_BINARY_DIR}")

# add include directory
include_directories(include)

# source directories
add_subdirectory(lib)
add_subdirectory(server)

# install idso1070.pc
install(FILES ${CMAKE_BINARY_DIR}/idso1070.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

# install include directory
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/idso1070)

# Include uninstall target helper
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/uninstall.cmake")