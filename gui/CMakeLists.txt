project (idso1070-gui)

# add gtkmm
find_package(Gtkmm REQUIRED)
include_directories(${GTKMM_INCLUDE_DIRS})

# add PThread
find_package(PThread REQUIRED)
include_directories(${PTHREAD_INCLUDE_DIRS})

add_executable(idso1070-gui main.cxx ProtocolWorker.cxx AppWindow.cxx)

target_link_libraries (idso1070-gui idso1070 ${GTKMM_LIBRARIES} ${PTHREAD_LIBRARIES})

# Copy glade file to binary dir for local testing
add_custom_command(
        TARGET idso1070-gui POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/gui/gui.glade
                ${CMAKE_BINARY_DIR}/gui/gui.glade)

# Install glade file to ${CMAKE_INSTALL_DATAROOTDIR}/idso1070-gui
install(FILES ${CMAKE_BINARY_DIR}/gui/gui.glade DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/idso1070-gui)

install (TARGETS idso1070-gui DESTINATION ${CMAKE_INSTALL_BINDIR})