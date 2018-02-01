#include "TransmissionLogDialog.h"
#include <iomanip>

TransmissionLogDialog::TransmissionLogDialog(BaseObjectType *cobject, const RefPtr<Builder> &refGlade)
    : Window(cobject),
      refGlade(refGlade),
      pTransmissionLogView(nullptr),
      pTransmissionLogBuffer(nullptr),
      transmissionCounter(1)
{
    // Get references to widgets from glade file
    refGlade->get_widget("textTransmissionLog", pTransmissionLogView);
    pTransmissionLogBuffer = RefPtr<TextBuffer>::cast_dynamic(refGlade->get_object("TransmissionLogBuffer"));

    // Connect signals with handlers
}

TransmissionLogDialog::~TransmissionLogDialog()
{
}

void TransmissionLogDialog::updateLog(Protocol::TransmissionLog &log)
{
    ustring logBuffer;
    for (auto transmission : log)
    {
        logBuffer += ustring::compose("<big>Transmission #%1</big>\n", transmissionCounter);
        logBuffer += "\n<b>Command:</b>\n";
        logBuffer += hexdump(transmission->command.getPayload(), Command::CommandSize);
        logBuffer += "\n<b>Response Header:</b>\n";
        logBuffer += hexdump(transmission->response.getHeader(), Packet::HeaderSize);
        logBuffer += "\n<b>Response Payload:</b>\n";
        logBuffer += hexdump(transmission->response.getPayload(), transmission->response.getPayloadLength());
        logBuffer += "\n<tt>--------------------------------------------------------------------------------------------------------------------</tt>\n\n\n";
        transmissionCounter++;
        delete transmission;
    }
    TextIter begin = pTransmissionLogBuffer->begin();
    pTransmissionLogBuffer->insert_markup(begin, logBuffer);
    log.clear();
}

ustring TransmissionLogDialog::hexdump(uint8_t *data, size_t length, int cols)
{
    ustring hexdump = "<tt>";
    int rowCount = (length / cols) + (length % cols != 0 ? 1 : 0);
    for (int row = 0; row < rowCount; row++)
    {
        int colCount = row == (length / cols) ? length % cols : cols;
        hexdump += ustring::compose("<b>0x%1</b> | ", ustring::format(setfill(L'0'), setw(8), std::hex, row * cols));
        for (int col = 0; col < colCount; col++)
        {
            unsigned int x = (unsigned int)data[(row * cols) + col] & 0xff;
            hexdump += ustring::compose("%1 ", ustring::format(setfill(L'0'), setw(2), std::hex, x));
            if ((col + 1) % 8 == 0)
                hexdump += "| ";
        }
        hexdump += "\n";
    }
    hexdump += "</tt>";
    return hexdump;
}