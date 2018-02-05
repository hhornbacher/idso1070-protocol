# Include resources helper
include("${CMAKE_SOURCE_DIR}/cmake/resources.cmake")

add_custom_target(gui-resources)

add_resource(glade_data gui.glade gui-resources)

add_library(resources ${RESOURCES_OBJECTS})
set_target_properties(resources PROPERTIES LINKER_LANGUAGE CXX)
add_dependencies(resources gui-resources)