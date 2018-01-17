project (idso1070-server)

# add libmicrohttpd
find_package(MHD)
include_directories(${MHD_INCLUDE_DIR})

add_executable(idso1070-server main.cxx StreamServer.cxx ControlServer.cxx)

target_link_libraries (idso1070-server idso1070 ${MHD_LIBRARIES})

install (TARGETS idso1070-server DESTINATION ${CMAKE_INSTALL_BINDIR})