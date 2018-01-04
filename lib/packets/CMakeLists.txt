add_library(packets Command.cxx CommandQueue.cxx CommandGenerator.cxx ResponsePacket.cxx)
target_link_libraries (packets)