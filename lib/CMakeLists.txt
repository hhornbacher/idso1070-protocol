set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(connection)
add_subdirectory(packets)

add_library(idso1070 SHARED Protocol.cxx IDSO1070.cxx util.cxx)
target_link_libraries (idso1070 ${Boost_LIBRARIES} connection packets)

set_target_properties(idso1070 PROPERTIES VERSION ${PROJECT_VERSION})

install (TARGETS idso1070 
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
