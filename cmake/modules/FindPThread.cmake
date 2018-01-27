find_path(PTHREAD_INCLUDE_DIR 
            NAMES pthread.h
            HINTS $ENV{PTHREAD_DIR}/include
            PATHS /usr/local/include
                  /usr/include )

find_library(PTHREAD_LIBRARY 
            NAMES pthread
            HINTS $ENV{PTHREAD_DIR}/lib
            PATHS /usr/local/lib
                  /usr/lib)

set(PTHREAD_LIBRARIES ${PTHREAD_LIBRARY} )
set(PTHREAD_INCLUDE_DIRS ${PTHREAD_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set PTHREAD_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(pthread  DEFAULT_MSG
                                  PTHREAD_LIBRARY PTHREAD_INCLUDE_DIR)

mark_as_advanced(PTHREAD_INCLUDE_DIR PTHREAD_LIBRARY )
