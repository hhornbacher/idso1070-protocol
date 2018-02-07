#include <ProtocolWorker.h>

#include <iostream>
#include <csignal>

ProtocolWorker worker;
bool run = true;

void signalHandler(int signal)
{
    if (signal == SIGINT)
    {
        cout << endl
             << endl;
        run = false;
    }
}

void notify()
{
    if (worker.isConnecting())
    {
        cout << "Connecting: " << worker.getProgress() * 100 << "%" << endl;
    }

    if (worker.isConnected())
    {
    }
    else
    {
        if (worker.isConnectionLost())
        {
            cout << "Connection error: " << worker.getConnectionLostReason() << endl;
        }
    }
}

int main(int argc, char *argv[])
{
    signal(SIGINT, signalHandler);

    worker.setNotifyHandler(notify);
    worker.start();
    worker.connect("/dev/ttyACM0");

    while (run)
    {
    }
    worker.stop();
    return 0;
}
