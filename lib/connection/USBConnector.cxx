#include "connection/USBConnector.h"

USBConnector::USBConnector(string device) : device(device), handle(-1)
{
}

USBConnector::~USBConnector()
{
    if (handle < 0)
        close(handle);
}

void USBConnector::enumerateDevices(USBDeviceList &list)
{
    DIR *dp;
    dirent *dirp;
    const char *path = "/sys/class/tty";
    if ((dp = opendir(path)) == NULL)
    {
        return;
    }

    while ((dirp = readdir(dp)) != NULL)
    {
        if (dirp->d_type == DT_LNK)
        {
            string currentPath = path;
            currentPath += "/";
            currentPath += dirp->d_name;
            currentPath += "/device";
            struct stat buffer;
            if (stat(currentPath.c_str(), &buffer) == 0)
            {
                string infoPath = currentPath;
                infoPath += "/modalias";
                int info = open(infoPath.c_str(), O_RDONLY);
                if (info != -1)
                {
                    char buffer[15];
                    int size = read(info, buffer, 14);
                    buffer[14] = 0;
                    close(info);
                    if (strcmp(buffer, "usb:v0483p5740") == 0)
                    {
                        string dev = "/dev/";
                        dev += dirp->d_name;
                        list.push_back(dev);
                    }
                }
            }
        }
    }
    closedir(dp);
}

void USBConnector::start()
{
    if (handle != -1)
        return;

    termios tty;
    handle = open(device.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
    if (!handle)
    {
        throw Exception("Cannot open device");
    }
    memset(&tty, 0, sizeof tty);
    int res = 0;
    if ((res = tcgetattr(handle, &tty)) != 0)
    {
        throw Exception("Cannot read serial port configuration");
    }

    cfsetospeed(&tty, USBSerialSpeed);
    cfsetispeed(&tty, USBSerialSpeed);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // 8-bit chars
    // disable IGNBRK for mismatched speed tests; otherwise receive break
    // as \000 chars
    tty.c_iflag &= ~IGNBRK; // disable break processing
    tty.c_lflag = 0;        // no signaling chars, no echo,
                            // no canonical processing
    tty.c_oflag = 0;        // no remapping, no delays
    tty.c_cc[VMIN] = 0;     // read doesn't block
    tty.c_cc[VTIME] = 5;    // 0.5 seconds read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

    tty.c_cflag |= (CLOCAL | CREAD);   // ignore modem controls,
                                       // enable reading
    tty.c_cflag &= ~(PARENB | PARODD); // shut off parity
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    tty.c_cc[VMIN] = 0;  // Set non-blocking
    tty.c_cc[VTIME] = 5; // 0.5 seconds read timeout

    if ((res = tcsetattr(handle, TCSANOW, &tty)) != 0)
    {
        throw Exception("Cannot set serial port configuration");
    }

    connected = true;
}

void USBConnector::stop()
{
    if (handle != -1)
        close(handle);
    connected = false;
    handle = -1;
}

ConnectorType USBConnector::getType()
{
    return CONNECTOR_USB;
}

void USBConnector::transmit(uint8_t *data, size_t length)
{
    if (handle == -1)
        return;
    int res = 0;
    if ((res = write(handle, data, length)) < 0)
    {
        stop();
        throw Exception("Connection lost");
    }
}

void USBConnector::receive()
{
    if (!rawBuffer.full())
    {
        size_t size = RawBufferLength - rawBuffer.size();
        uint8_t localBuffer[size];
        if (handle == -1)
            return;
        ssize_t receivedSize = read(handle, localBuffer, size);
        if (receivedSize < 0)
        {
            stop();
            throw Exception("Connection lost");
        }
        else if (receivedSize > 0)
        {
            for (ssize_t i = 0; i < receivedSize; i++)
                rawBuffer.push_back(localBuffer[i]);
        }
    }
    grabPacket();
}