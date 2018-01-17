project (idso1070a-server)

# add libmicrohttpd
find_package(MHD)
include_directories(${MHD_INCLUDE_DIR})

add_executable(idso1070a-server main.cxx)

target_link_libraries (idso1070a-server idso1070a ${MHD_LIBRARIES})

install (TARGETS idso1070a-server DESTINATION ${CMAKE_INSTALL_BINDIR})