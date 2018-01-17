set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(device)
add_subdirectory(connection)
add_subdirectory(packets)
add_subdirectory(util)

add_library(idso1070 SHARED Protocol.cxx)
target_link_libraries (idso1070 ${Boost_LIBRARIES} device connection packets util)

set_target_properties(idso1070 PROPERTIES VERSION ${PROJECT_VERSION})

install (TARGETS idso1070 
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
