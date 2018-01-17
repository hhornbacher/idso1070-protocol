#include "Server.h"

void sigHandler(int sig);

class Main
{
  private:
    bool runProgram = true;
    Server server;

  public:
    Main()
    {
        signal(SIGINT, sigHandler);
    }

    void stop()
    {
        runProgram = false;
        server.stop();
    }

    void onProgress(float progress)
    {
        printf("\nProgress: %.2f\n", (progress * 100.0f));
    }

    int run()
    {
        server.start();

        while (runProgram)
        {
            server.process();
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
        printf("\nExiting!\n\n");
        program.stop();
        exit(0);
    }
}