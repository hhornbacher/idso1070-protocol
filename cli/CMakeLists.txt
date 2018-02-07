project (idso1070-cli)

# add liquidsdr
find_package(LiquidSdr REQUIRED)
include_directories(${LIQUIDDSP_INCLUDE_DIRS})

add_executable(idso1070-cli main.cxx)
target_link_libraries (idso1070-cli idso1070 ${LIQUIDDSP_LIBRARIES})

install (TARGETS idso1070-cli DESTINATION ${CMAKE_INSTALL_BINDIR})