#ifndef _TIMEOUT_H_
#define _TIMEOUT_H_

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <inttypes.h>

class Timeout
{
private:
  int timeout;
  long nextTick;

  long getTicks();

public:
  Timeout(int ms);

  void reset();
  bool isTimedOut();
};

#endif // _TIMEOUT_H_