add_library(connection Connector.cxx TCPConnector.cxx USBConnector.cxx)
target_link_libraries (connection)