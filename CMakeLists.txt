cmake_minimum_required (VERSION 3.5)

# version numbers
set (libidso1070a_VERSION_MAJOR 1)
set (libidso1070a_VERSION_MINOR 0)

project (libidso1070a VERSION "${libidso1070a_VERSION_MAJOR}.${libidso1070a_VERSION_MINOR}")
 
# create config.h
configure_file (
  "config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
)

# create idso1070a.pc
configure_file(idso1070a.pc.in idso1070a.pc @ONLY)

# default GNU install directories
include(GNUInstallDirs)

# set buil to debug
set(CMAKE_BUILD_TYPE Debug)

# set C++ standard to C++11
set (CMAKE_CXX_STANDARD 11)

# add local cmake dir to module path
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)

# add boost as dependency and configure
add_definitions(-DBOOST_FILESYSTEM_NO_DEPRECATED)
find_package(Boost 1.58.0 COMPONENTS filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

# add binary dir as include directory, to be able to include config.h
include_directories("${PROJECT_BINARY_DIR}")

# add include directory
include_directories(include)

# source directories
add_subdirectory(lib)
add_subdirectory(server)

# install idso1070a.pc
install(FILES ${CMAKE_BINARY_DIR}/idso1070a.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

# install include directory
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/idso1070a)

# uninstall target
# https://cmake.org/Wiki/CMake_FAQ#Can_I_do_.22make_uninstall.22_with_CMake.3F
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)
add_custom_target(uninstall
    ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
)