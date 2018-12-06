//
// Created by root on 11/30/18.
//

#ifndef CODEBASE_APPEND_FILE_H
#define CODEBASE_APPEND_FILE_H

#include <base/types/types.h>
#include <base/string/adapted_string.h>

#include <stdio.h>

class AppendFile
{
public:
    AppendFile(AdaptedString path, AdaptedString filename);
    ~AppendFile();

    void append(const char *data, size_t len);

    void flush();

    off_t writtenBytes();

private:
    static const int kFileBufferSize = 64 * 1024;
    char buffer_[kFileBufferSize];
    string file_path_;
    FILE *fp_;
    off_t written_bytes_;

    size_t write(const char *data, size_t len);
};

#endif //CODEBASE_APPEND_FILE_H
