#include "IDSO1070A.h"

bool IDSO1070A::isSampleRate200Mor250M()
{
    return timeBase <= HDIV_1uS;
}