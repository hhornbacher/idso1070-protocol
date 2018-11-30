#include "util.h"

#include <sstream>
#include <iomanip>

using namespace std;

// float mapValue(int i, float f, float f2, float f3, float f4)
// {
//     return (((((float)i) - f) / (f2 - f)) * (f4 - f3)) + f3;
// }

// float mapValue(float f, float f2, float f3, float f4, float f5)
// {
//     return (((f - f2) / (f3 - f2)) * (f5 - f4)) + f4;
// }

string hexdump(const uint8_t *data, size_t length, int cols)
{
    stringstream ss;
    int rowCount = (length / cols) + (length % cols != 0 ? 1 : 0);
    for (int row = 0; row < rowCount; row++)
    {
        int colCount = row == ((int)length / cols) ? length % cols : cols;

        ss << "0x" << setfill('0') << setw(8) << hex << (row * cols) << " | ";

        for (int col = 0; col < colCount; col++)
        {
            unsigned int x = (unsigned int)data[(row * cols) + col] & 0xff;

            ss << setfill('0') << setw(2) << hex << x << " ";
            if ((col + 1) % 8 == 0)
                ss << "| ";
        }
        ss << endl;
    }
    ss << endl;
    return ss.str();
}