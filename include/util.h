#ifndef _UTIL_H_
#define _UTIL_H_

#include <string>

using namespace std;

float mapValue(int i, float f, float f2, float f3, float f4);
float mapValue(float f, float f2, float f3, float f4, float f5);
string hexdump(uint8_t *data, size_t length, int cols);

#endif // _UTIL_H_