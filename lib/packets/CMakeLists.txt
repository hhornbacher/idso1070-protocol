add_library(packets Command.cxx CommandFactory.cxx Packet.cxx Sample.cxx Response.cxx PacketParser.cxx)
target_link_libraries (packets)