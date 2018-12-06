//
// Created by root on 12/6/18.
//

#include <base/logging/logging_file.h>
#include <base/time/timestamp.h>

int main(int argc, const char *argv[])
{
    char buffer[1024];
    LoggingFile *logger = new LoggingFile("./", "logging_test");

    for (int i = 0; i < 100000000; i++)
    {
        snprintf(buffer, sizeof(buffer), "%s\n", TimeStamp::now().toFormattedString().c_str());
        logger->append(buffer, strlen(buffer));
    }

    return 0;
}