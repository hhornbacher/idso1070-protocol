#ifndef _HEXDUMP_H_
#define _HEXDUMP_H_

#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstring>

#define HEXDUMP_COLS 32

void hexdump(uint8_t *data, size_t length);

#endif // _HEXDUMP_H_