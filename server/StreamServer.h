#ifndef _STREAM_SERVER_H_
#define _STREAM_SERVER_H_

#include "Protocol.h"

class StreamServer
{
private:
  Channel &channel;
  int port;

public:
  StreamServer(Channel &channel, int port);

  void start();
  void stop();
  void process();
};

#endif // _STREAM_SERVER_H_