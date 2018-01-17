#ifndef _STREAM_SERVER_H_
#define _STREAM_SERVER_H_

#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstring>

#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>

#include "Protocol.h"

class StreamServer
{
private:
  Channel &channel;
  int port;

  int serverSocket = -1;

  int clientSocket = -1;
  sockaddr_in clientAddress;

public:
  StreamServer(Channel &channel, int port);

  void start();
  void stop();
  void process();
};

#endif // _STREAM_SERVER_H_