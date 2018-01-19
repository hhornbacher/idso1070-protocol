project (idso1070-server)

# add libmicrohttpd
find_package(MHD)
include_directories(${MHD_INCLUDE_DIR})

# add cppnetlib
find_package(cppnetlib 0.11 REQUIRED)
INCLUDE_DIRECTORIES(${CPPNETLIB_INCLUDE_DIRS})

add_executable(idso1070-server main.cxx StreamServer.cxx HttpServer.cxx ControlServer.cxx)

target_link_libraries (idso1070-server idso1070 ${CPPNETLIB_LIBRARIES} ${MHD_LIBRARIES})

install (TARGETS idso1070-server DESTINATION ${CMAKE_INSTALL_BINDIR})