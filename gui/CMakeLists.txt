project (idso1070-gui)

# add gtkmm
find_package(Gtkmm REQUIRED)
include_directories(${GTKMM_INCLUDE_DIRS})

# add PThread
find_package(PThread REQUIRED)
include_directories(${PTHREAD_INCLUDE_DIRS})

# Generate object files from resource binary data
set(GLADE_OBJECT_FILE ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/idso1070-gui-resources.dir/glade_data.o)
set(SCOPE_CSS_OBJECT_FILE ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/idso1070-gui-resources.dir/scope_css.o)
add_custom_target(idso1070-gui-resources)
add_custom_command(
    TARGET idso1070-gui-resources
    PRE_BUILD
    COMMAND cp gui.glade glade_data
    COMMAND dd if=/dev/zero bs=1 count=1 >> glade_data 2> /dev/null
    COMMAND objcopy --input binary --output elf64-x86-64 --binary-architecture i386:x86-64 --rename-section .data=.rodata,CONTENTS,ALLOC,LOAD,READONLY,DATA glade_data ${GLADE_OBJECT_FILE}
    COMMAND rm glade_data
    BYPRODUCTS ${GLADE_OBJECT_FILE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_custom_command(
    TARGET idso1070-gui-resources
    PRE_BUILD
    COMMAND cp scope.css scope_css
    COMMAND dd if=/dev/zero bs=1 count=1 >> scope_css 2> /dev/null
    COMMAND objcopy --input binary --output elf64-x86-64 --binary-architecture i386:x86-64 --rename-section .data=.rodata,CONTENTS,ALLOC,LOAD,READONLY,DATA scope_css ${SCOPE_CSS_OBJECT_FILE}
    COMMAND rm scope_css
    BYPRODUCTS ${SCOPE_CSS_OBJECT_FILE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(idso1070-gui main.cxx ProtocolWorker.cxx ScopeWidget.cxx SettingsWidget.cxx AppWindow.cxx TransmissionLogDialog.cxx)
target_link_libraries (idso1070-gui idso1070 ${GLADE_OBJECT_FILE} ${SCOPE_CSS_OBJECT_FILE} ${GTKMM_LIBRARIES} ${PTHREAD_LIBRARIES})
add_dependencies(idso1070-gui idso1070-gui-resources)

install (TARGETS idso1070-gui DESTINATION ${CMAKE_INSTALL_BINDIR})