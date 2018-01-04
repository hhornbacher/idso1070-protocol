project (libidso1070a)

# The version number.
set (libidso1070a_VERSION_MAJOR 1)
set (libidso1070a_VERSION_MINOR 0)
 
configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
)
 
# Add binary dir as include directory, to be able to include config.h
include_directories("${PROJECT_BINARY_DIR}")

add_subdirectory(device)
add_subdirectory(connection)
add_subdirectory(packets)
add_subdirectory(util)

add_library(idso1070a Protocol.cxx)
target_link_libraries (idso1070a device connection packets util)

install (TARGETS idso1070a DESTINATION lib)