project (idso1070a-example)

add_executable(idso1070a-example main.cxx)

target_link_libraries (idso1070a-example idso1070a)

install (TARGETS idso1070a-example DESTINATION ${CMAKE_INSTALL_BINDIR})