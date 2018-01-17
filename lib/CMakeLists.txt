set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(device)
add_subdirectory(connection)
add_subdirectory(packets)
add_subdirectory(util)

add_library(idso1070a SHARED Protocol.cxx)
target_link_libraries (idso1070a ${Boost_LIBRARIES} device connection packets util)

set_target_properties(idso1070a PROPERTIES VERSION ${PROJECT_VERSION})

install (TARGETS idso1070a 
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
