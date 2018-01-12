add_library(util hexdump.cxx Timeout.cxx mapValue.cxx Serializer.cxx FIFOSerializer.cxx)
target_link_libraries (util)