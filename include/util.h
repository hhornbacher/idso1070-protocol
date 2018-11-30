#pragma once

#include <string>

float mapValue(int i, float f, float f2, float f3, float f4);
float mapValue(float f, float f2, float f3, float f4, float f5);
std::string hexdump(const uint8_t *data, size_t length, int cols);
