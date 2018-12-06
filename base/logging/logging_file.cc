//
// Created by root on 11/29/18.
//

#include <base/logging/logging_file.h>

#include <base/file/append_file.h>
#include <base/time/timestamp.h>

LoggingFile::LoggingFile(AdaptedString logging_path, AdaptedString logging_basename, bool thread_safety, int flush_interval, int scroll_size) :
        logging_path_(logging_path.c_str()),
        logging_basename_(logging_basename.c_str()),
        flush_interval_(flush_interval),
        scroll_size_(scroll_size),
        logging_times_(0),
        last_scroll_(0),
        lock_(thread_safety ? new MutexLock : nullptr)
{
    scrollLoggingFile();
}

void LoggingFile::flush()
{
    if (lock_)
    {
        ScopedLock lock(*lock_);
        file_->flush();
    }
    else
    {
        file_->flush();
    }
}

void LoggingFile::append(const char *line, size_t len)
{
    if (lock_)
    {
        ScopedLock lock(*lock_);
        append_unlocked(line, len);
    }
    else
    {
        append_unlocked(line, len);
    }
}

void LoggingFile::append_unlocked(const char *line, size_t len)
{
    file_->append(line, len);

    if (file_->writtenBytes() > scroll_size_)
    {
        scrollLoggingFile();
    }
    else
    {
        logging_times_ ++;
        if (logging_times_ >= kCheckPerLoggingTimes)
        {
            logging_times_ = 0;
            TimeStamp now = TimeStamp::now();
            if (now.seconds() / kSecondsPerDaily != last_scroll_ / kSecondsPerDaily)
            {
                scrollLoggingFile();
            }
            else if (now.seconds() - last_scroll_ > flush_interval_)
            {
                file_->flush();
            }
        }
    }
}

void LoggingFile::scrollLoggingFile()
{
    string filename = getScrollLoggingFilename();

    last_scroll_ = TimeStamp::now().seconds();

    file_.reset(new AppendFile(logging_path_, filename));
}

string LoggingFile::getScrollLoggingFilename()
{
    string filename;

    filename.reserve(256);
    filename += TimeStamp::now().toAssignedFotmattedString("%Y%m%d-%H%M%S") + ".";

    char pid[16] = {0};
    snprintf(pid, sizeof(pid), "%d", getpid());

    filename += string(pid) + "." + logging_basename_ + ".log";

    return filename;
}