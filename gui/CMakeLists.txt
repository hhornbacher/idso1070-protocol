project (idso1070-gui)

# add gtkmm
find_package(Gtkmm REQUIRED)
include_directories(${GTKMM_INCLUDE_DIRS})

# add gtkmm-plplot
find_package(Gtkmm-PLplot REQUIRED)
include_directories(${GTKMM_PLPLOT_INCLUDE_DIRS})

# add PThread
find_package(PThread REQUIRED)
include_directories(${PTHREAD_INCLUDE_DIRS})

# Generate c array from glade file
add_custom_target(idso1070-gui-glade)
add_custom_command(
    TARGET idso1070-gui-glade
    PRE_BUILD
    COMMAND cp gui.glade glade_data
    COMMAND dd if=/dev/zero bs=1 count=1 >> glade_data 2> /dev/null
    COMMAND objcopy --input binary --output elf64-x86-64 --binary-architecture i386:x86-64 --rename-section .data=.rodata,CONTENTS,ALLOC,LOAD,READONLY,DATA glade_data glade_data.o
    COMMAND rm glade_data
    BYPRODUCTS gui/glade_data.o
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(idso1070-gui main.cxx ProtocolWorker.cxx GraphWidget.cxx SettingsWidget.cxx AppWindow.cxx TransmissionLogDialog.cxx)
target_link_libraries (idso1070-gui idso1070 ${CMAKE_CURRENT_SOURCE_DIR}/glade_data.o ${GTKMM_LIBRARIES} ${GTKMM_PLPLOT_LIBRARIES} ${PTHREAD_LIBRARIES})
add_dependencies(idso1070-gui idso1070-gui-glade)

# add_custom_command(
#     TARGET idso1070-gui
#     POST_BUILD
#     COMMAND rm glade_data.o
#     WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/gui)

install (TARGETS idso1070-gui DESTINATION ${CMAKE_INSTALL_BINDIR})