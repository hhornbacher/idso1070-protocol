#include "TransmissionLogDialog.h"
#include <iomanip>

TransmissionLogDialog::TransmissionLogDialog(BaseObjectType *cobject, const RefPtr<Builder> &refGlade)
    : Window(cobject),
      refGlade(refGlade),
      pTransmissionLogBuffer(nullptr)
{
    // Get references to widgets from glade file
    pTransmissionLogBuffer = RefPtr<TextBuffer>::cast_dynamic(refGlade->get_object("TransmissionLogBuffer"));

    // Connect signals with handlers
}

TransmissionLogDialog::~TransmissionLogDialog()
{
}

void TransmissionLogDialog::updateLog(Protocol::TransmissionLog &log)
{
    ustring logBuffer;
    int i = 1;
    for (auto transmission : log)
    {
        logBuffer += ustring::compose("Transmission #%1\n", i);
        logBuffer += "Command:\n";
        logBuffer += hexdump(transmission->command.getPayload(), Command::CommandSize);
        logBuffer += "Response:\n";
        logBuffer += hexdump(transmission->response.getHeader(), Packet::PacketSize);
        logBuffer += ustring::compose("\n\n", i);
        i++;
    }
    pTransmissionLogBuffer->set_text(logBuffer);
}

ustring TransmissionLogDialog::hexdump(uint8_t *data, size_t length, int cols)
{
    ustring hexdump;
    int rowCount = (length / cols) + (length % cols != 0 ? 1 : 0);
    for (int row = 0; row < rowCount; row++)
    {
        int colCount = row == (length / cols) ? length % cols : cols;
        hexdump += ustring::compose("0x%1: ", ustring::format(setfill(L'0'), setw(8), std::hex, row * cols));
        for (int col = 0; col < colCount; col++)
        {
            unsigned int x = (unsigned int)data[(row * cols) + col] & 0xff;
            hexdump += ustring::compose("%1 ", ustring::format(setfill(L'0'), setw(2), std::hex, x));
        }
        hexdump += "\n";
    }
    return hexdump;
}