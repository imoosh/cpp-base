//
// Created by root on 11/30/18.
//

#include <base/file/append_file.h>

AppendFile::AppendFile(AdaptedString path, AdaptedString filename) :
    file_path_(string(path.c_str()) + "/" + string(filename.c_str())),
    fp_(fopen(file_path_.c_str(), "ae")),   // 'e' for O_CLOEXEC
    written_bytes_(0)
{
    cout << file_path_ << endl;
    ASSERT(fp_);

    setbuffer(fp_, buffer_, kFileBufferSize);
}

AppendFile::~AppendFile()
{
    fclose(fp_);
}

void AppendFile::append(const char *data, size_t len)
{
    size_t write_bytes = 0, write_count = 0;

    do {
        write_bytes = write(data + write_count, len - write_count);
        if (write_bytes <= 0)
        {
            if (int errnum = ferror(fp_))
            {
                fprintf(stderr, "write failed. %s\n", strerror_tl(errnum));
            }
            break;
        }

        write_count += write_bytes;
    } while (write_count < len);

    written_bytes_ += len;
}

size_t AppendFile::write(const char *data, size_t len)
{
    return fwrite_unlocked(data, 1, len, fp_);
}

void AppendFile::flush()
{
    fflush(fp_);
}

off_t AppendFile::writtenBytes()
{
    return written_bytes_;
}