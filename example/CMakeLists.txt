project (idso1070a-example)

# Add libidso1070a include directory
if(NOT libidso1070a_SOURCE_DIR)
find_package(LIBIDSO1070A REQUIRED)
include_directories(${LIBIDSO1070A_INCLUDE_DIR})
else()
include_directories(${libidso1070a_SOURCE_DIR})
endif()

add_executable(idso1070a-example main.cxx)

target_link_libraries (idso1070a-example idso1070a)

install (TARGETS idso1070a-example DESTINATION bin)