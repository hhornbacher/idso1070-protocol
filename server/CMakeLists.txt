project (idso1070-server)

# add Poco
find_package(Poco REQUIRED)
include_directories(${POCO_INCLUDE_DIR})

add_executable(idso1070-server main.cxx StreamServer.cxx HttpServer.cxx ControlServer.cxx)

target_link_libraries (idso1070-server idso1070 ${POCO_NET_LIBRARY} ${POCO_JSON_LIBRARY} ${POCO_FOUNDATION_LIBRARY})

install (TARGETS idso1070-server DESTINATION ${CMAKE_INSTALL_BINDIR})