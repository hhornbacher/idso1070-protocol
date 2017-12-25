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

add_library(idso1070a Protocol.cxx Connector.cxx CommandsGenerator.cxx ResponsePacket.cxx hexdump.cxx)

install (TARGETS idso1070a DESTINATION lib)