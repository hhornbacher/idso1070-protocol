#include <cstdlib>
#include <cstdio>
#include <csignal>
#include <unistd.h>

#include "USBConnector.h"
#include "TCPConnector.h"
#include "Protocol.h"

void sigHandler(int sig);

class Main : public ResponseHandler
{
  private:
    bool runProgram = true;
    const char *device = "/dev/ttyACM0";

    int resendCounter = 0;
    // const char *serverIP = "192.168.1.1";
    // const uint16_t serverPort = 8870;

    // TCPConnector connection((char *)serverIP, serverPort);
    USBConnector connection;
    Protocol protocol;

  public:
    Main() : connection((char *)device), protocol(&connection, this)
    {
        signal(SIGINT, sigHandler);
    }

    bool onResponse(Command *cmd, bool success)
    {
        if (success)
        {
            resendCounter = 0;
            printf("Success, next command.\n");
            protocol.removeLastCommand();
        }
        else
        {
            if (resendCounter < 3)
            {
                printf("Resending: ");
                cmd->print();
                protocol.resendLastCommand();
                resendCounter++;
            }
            else
            {
                printf("Problem while sending: ");
                cmd->print();
                printf("Exiting!\n\n");
                exit(0);
            }
        }
        return true;
    }

    void stop()
    {
        runProgram = false;
    }

    int run()
    {

        protocol.start();
        protocol.sendCommands(protocol.getCmdGen().readFPGAVersion());

        while (runProgram)
        {
            protocol.process();
        }
        return EXIT_SUCCESS;
    }
} program;

int main(int argc, char **argv)
{
    return program.run();
}

void sigHandler(int sig)
{
    if (sig == SIGINT)
    {
        program.stop();
        exit(0);
    }
}