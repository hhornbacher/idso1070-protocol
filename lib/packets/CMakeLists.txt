add_library(packets Command.cxx CommandFactory.cxx Response.cxx ResponseParser.cxx)
target_link_libraries (packets)