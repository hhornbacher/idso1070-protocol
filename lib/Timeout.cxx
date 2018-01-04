#include "Timeout.h"

Timeout::Timeout(int ms) : timeout(ms)
{
    nextTick = getTicks() + ms;
}

long Timeout::getTicks()
{
    long ms;  // Milliseconds
    time_t s; // Seconds
    struct timespec spec;

    clock_gettime(CLOCK_REALTIME, &spec);

    s = spec.tv_sec;
    ms = round(spec.tv_nsec / 1.0e6); // Convert nanoseconds to milliseconds
    if (ms > 999)
    {
        s++;
        ms = 0;
    }
    return (s * 1000) + ms;
}

void Timeout::reset()
{
    nextTick = getTicks() + timeout;
}

bool Timeout::isTimedOut()
{
    if (getTicks() > nextTick)
        return true;
    return false;
}