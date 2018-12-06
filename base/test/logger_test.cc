//
// Created by root on 12/3/18.
//

#include <base/logging/logger.h>
#include <base/logging/logging_file.h>
#include <base/logging/async_logging.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char *argv[])
{
    AsyncLogging *logger = Logger<AsyncLogging>::getLogger("./", "singleton", 64, 3);
    logger->start();
    logger->append(string("abcdefg123456"));

    LoggingFile *logger1 = Logger<LoggingFile>::getLogger("./");
    string line("123456abcdefg");
    logger1->append(line.c_str(), line.length());

    // 注意：会得到另外一个对象，并不是单例模式。
//    AsyncLogging *logger1 = Logger<AsyncLogging>::getLogger();
//    ASSERT(logger1);
//    cout << logger1 << endl;

//    logger->append("abcdef 123456\n");

    getchar();

    return 0;
}
