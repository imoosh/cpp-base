//
// Created by root on 12/6/18.
//

#ifndef CODEBASE_ADAPTEDSTRING_H
#define CODEBASE_ADAPTEDSTRING_H

#include <base/types/types.h>
#include <boost/noncopyable.hpp>

class AdaptedString
{
public:
    AdaptedString(const char *str) : str_(str), size_(strlen(str)) {}
    AdaptedString(const string &str) : str_(str.c_str()), size_(str.length()) {}

    AdaptedString operator=(const char *str)
    {
        AdaptedString as(str);
        return as;
    }

    AdaptedString(const char *str, int size) : str_(str), size_(static_cast<size_t>(size)) {}
    AdaptedString(const char *str, size_t size) : str_(str), size_(size) {}

    const char *c_str()
    {
        return str_;
    }

    size_t length()
    {
        return size_;
    }

private:
    const char *str_;
    size_t size_;
};

#endif //CODEBASE_ADAPTEDSTRING_H
