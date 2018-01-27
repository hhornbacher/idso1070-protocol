project (idso1070-gui)

# add gtkmm
find_package(Gtkmm REQUIRED)
include_directories(${GTKMM_INCLUDE_DIRS})

add_executable(idso1070-gui main.cxx)

target_link_libraries (idso1070-gui idso1070 ${GTKMM_LIBRARIES})

install (TARGETS idso1070-gui DESTINATION ${CMAKE_INSTALL_BINDIR})