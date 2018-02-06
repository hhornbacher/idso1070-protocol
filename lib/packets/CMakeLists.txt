add_library(packets Command.cxx CommandFactory.cxx Packet.cxx Sample.cxx Response.cxx)
target_link_libraries (packets)