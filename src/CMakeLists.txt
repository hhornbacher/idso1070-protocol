project (idso1070a-protocol)
 
# Add binary dir as include directory, to be able to include config.h
include_directories("${PROJECT_BINARY_DIR}")

# Add libidso1070a include directory
if(NOT libidso1070a_SOURCE_DIR)
find_package(LIBIDSO1070A REQUIRED)
include_directories(${LIBIDSO1070A_INCLUDE_DIR})
else()
include_directories(${libidso1070a_SOURCE_DIR})
endif()

add_executable(idso1070a-protocol main.cxx)

target_link_libraries (idso1070a-protocol idso1070a ${PTHREAD_LIBRARIES})

install (TARGETS idso1070a-protocol DESTINATION bin)