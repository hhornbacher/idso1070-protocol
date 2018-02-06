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

# add binary dir as include directory, to be able to include config.h
include_directories("${PROJECT_BINARY_DIR}")


set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(connection)
add_subdirectory(packets)

add_library(idso1070 SHARED Protocol.cxx ProtocolWorker.cxx IDSO1070.cxx util.cxx)
target_link_libraries (idso1070 ${Boost_LIBRARIES} connection packets)

set_target_properties(idso1070 PROPERTIES VERSION ${PROJECT_VERSION})

install (TARGETS idso1070 
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
