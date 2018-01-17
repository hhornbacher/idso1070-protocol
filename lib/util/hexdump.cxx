#include "util/hexdump.h"

void hexdump(uint8_t *data, size_t length)
{
    int rowCount = (length / HEXDUMP_COLS) + (length % HEXDUMP_COLS != 0 ? 1 : 0);
    for (int row = 0; row < rowCount; row++)
    {
        int colCount = row == (length / HEXDUMP_COLS) ? length % HEXDUMP_COLS : HEXDUMP_COLS;
        printf("0x%08x: ", row * HEXDUMP_COLS);
        for (int col = 0; col < colCount; col++)
        {
            unsigned int x = (unsigned int)data[(row * HEXDUMP_COLS) + col] & 0xff;
            printf("%02x ", x);
        }
        printf("\n");
    }
}