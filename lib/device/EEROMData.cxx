#include "EEROMData.h"

void EEROMData::print()
{
    printf(
        "[EEROMData]\n"
        "diffFixData[0]:\n");
    hexdump(diffFixData[0], 256);
    printf("diffFixData[1]:\n");
    hexdump(diffFixData[1], 256);
    printf("caliLevel:\n");
    hexdump(caliLevel, 200);
    printf("fpgaAlert:\n");
    hexdump(fpgaAlert, 40);
    printf(
        "productName = %s\n"
        "userName = %s\n"
        "\n\n",
        productName, userName);
}