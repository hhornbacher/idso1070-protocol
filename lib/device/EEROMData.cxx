#include "device/EEROMData.h"

void EEROMData::print()
{
    printf(
        "[EEROMData]\n"
        "productName = %s\n"
        "userName = %s\n",
        productName, userName);
    printf("caliLevel:\n");
    for (int i = 0; i < 100; i++)
    {
        printf("caliLevel[%d] = %d\n", i, caliLevel[i]);
    }
    printf("diffFixData[0]:\n");
    hexdump(diffFixData[0], 256);
    printf("diffFixData[1]:\n");
    hexdump(diffFixData[1], 256);
    printf("fpgaAlert:\n");
    hexdump(fpgaAlert, 40);
    printf("\n\n");
}