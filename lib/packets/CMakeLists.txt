add_library(packets Command.cxx CommandQueue.cxx CommandGenerator.cxx Response.cxx)
target_link_libraries (packets)