add_library(packets Command.cxx CommandFactory.cxx Packet.cxx Sample.cxx SampleParser.cxx Response.cxx ResponseParser.cxx)
target_link_libraries (packets)