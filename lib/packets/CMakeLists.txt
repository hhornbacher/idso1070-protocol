add_library(packets Command.cxx CommandGenerator.cxx Response.cxx ResponseParser.cxx)
target_link_libraries (packets)