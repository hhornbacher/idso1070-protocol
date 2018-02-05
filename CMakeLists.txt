cmake_minimum_required (VERSION 3.5)

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

# add include directory
include_directories(include)

# source directories
add_subdirectory(lib)
add_subdirectory(gui)

# install idso1070.pc
install(FILES ${CMAKE_BINARY_DIR}/idso1070.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

# install include directory
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/idso1070)

# Include uninstall target helper
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/uninstall.cmake")