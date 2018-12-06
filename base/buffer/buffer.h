//
// Created by win on 11/15/18.
//

#include <base/types/types.h>
#include <base/string/adapted_string.h>

#ifndef BASE_BUFFER_H
#define BASE_BUFFER_H

class Buffer
{
public:
    explicit Buffer(size_t size);
    ~Buffer();

    void append(const char *ptr, size_t len);
    void append(AdaptedString&);
    void bzero();
    void reset();
    size_t left();
    const char *data();
    const char *current();
    const char *end();
    size_t length();

private:
    char    *data_;
    char    *cur_;
    size_t  size_;
};

#endif //BASE_BUFFER_H
