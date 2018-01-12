#include "Serializer.h"

Serializer::Serializer(const char *path)
{
    if (path != NULL)
    {
        int length = strlen(path);
        if (length > 0)
        {
            printf("Opening file (creating if neccessary)...");
            fileHandle = open(path, O_CREAT | O_WRONLY);
            printf("OK\n");
        }
        else
            printf("Serializer: Error no path specified!\n");
    }
}

void Serializer::serialize(const void *data, size_t length)
{
    write(fileHandle, data, length);
}