//
// Created by root on 12/3/18.
//

#include <base/logging/async_logging.h>
#include <base/logging/logger.h>
#include <base/time/timestamp.h>
#include <base/string/adapted_string.h>

int main(int argc, const char *argv[])
{
//    AsyncLogging *global_logger = new AsyncLogging("./", "test");
    AsyncLogging *global_logger = Logger<AsyncLogging>::getLogger();
    global_logger->start();

    // 注意：会得到另外一个日志对象，并不是单例模式。
    // AsyncLogging *logger1 = Logger<AsyncLogging>::getLogger();

    while (true)
    {
//        char buffer[1024];
//        snprintf(buffer, sizeof(buffer), "%s ABCDEFGHIJKLMNOPQRTSUVWXYZ abcdefghijklmnopqrtsuvwxyz 1234567890 !@#$^&*()_+\n", TimeStamp::now().toFormattedString().c_str());
//        snprintf(buffer, sizeof(buffer), "%s\n", TimeStamp::now().toFormattedString().c_str());
        global_logger->append(TimeStamp::now().toFormattedString());
    }

    getchar();

    return 0;
}