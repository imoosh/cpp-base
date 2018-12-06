//
// Created by root on 11/29/18.
//

#ifndef CODEBASE_LOGGING_FILE_H
#define CODEBASE_LOGGING_FILE_H

#include <base/types/types.h>
#include <base/thread/mutexlock.h>
#include <base/file/append_file.h>
#include <base/string/adapted_string.h>

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

class LoggingFile : boost::noncopyable
{
public:
    LoggingFile(AdaptedString logging_path=".", AdaptedString logging_basename="", bool thread_safety = true, int flush_interval=3, int scroll_size=(100 * kBytesPerMegabyte));
    ~LoggingFile() = default;

    void flush();
    void append(const char *line, size_t len);
    void append_unlocked(const char *line, size_t len);

    void scrollLoggingFile();
    string getScrollLoggingFilename();

private:
    static const int kSecondsPerDaily = (24 * 60 * 60);
    static const int kCheckPerLoggingTimes = 1024;
    static const int kBytesPerMegabyte = (1 << 20);

    const string logging_path_;
    const string logging_basename_;
    int flush_interval_;
    int scroll_size_;
    int logging_times_;
    int last_scroll_;

    boost::scoped_ptr<MutexLock> lock_;
    boost::scoped_ptr<AppendFile> file_;
};

#endif //CODEBASE_LOGGING_FILE_H
