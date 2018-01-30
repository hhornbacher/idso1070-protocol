#include "util.h"

float mapValue(int i, float f, float f2, float f3, float f4)
{
    return (((((float)i) - f) / (f2 - f)) * (f4 - f3)) + f3;
}

float mapValue(float f, float f2, float f3, float f4, float f5)
{
    return (((f - f2) / (f3 - f2)) * (f5 - f4)) + f4;
}