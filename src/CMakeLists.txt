add_executable(idso1070a-protocol main.cxx Protocol.cxx Connector.cxx CommandsGenerator.cxx ResponsePacket.cxx)

target_link_libraries (idso1070a-protocol ${PTHREAD_LIBRARIES})

install (TARGETS idso1070a-protocol DESTINATION bin)