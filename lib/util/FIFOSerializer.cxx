#include "FIFOSerializer.h"

FIFOSerializer::FIFOSerializer(const char *path, bool enabled) : Serializer(NULL), enabled(enabled)
{
    int length = strlen(path);
    if (length > 0)
    {
        if (enabled)
        {
            printf("Creating FIFO file if neccessary...");
            struct stat st;
            if (stat(path, &st) != 0)
                mkfifo(path, 0666);
            printf("OK\n");

            printf("Waiting for FIFO reader...");
            fileHandle = open(path, O_WRONLY);
            printf("OK\n");
        }
        else
            printf("Disabled Serializer: %s!\n", path);
    }
    else
        printf("Serializer: Error no path specified!\n");
}

void FIFOSerializer::serialize(const void *data, size_t length)
{
    if (enabled)
        write(fileHandle, data, length);
}