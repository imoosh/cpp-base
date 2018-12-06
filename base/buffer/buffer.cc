//
// Created by win on 11/15/18.
//

#include <base/buffer/buffer.h>

#include <strings.h>

Buffer::Buffer(size_t size)
    : size_(size)
{
    data_ = new char[size];
    reset();
    bzero();
}

Buffer::~Buffer()
{
    cur_ = data_;
    delete data_;
}

size_t Buffer::left()
{
    return static_cast<size_t>(end() - cur_);
}

void Buffer::reset()
{
    cur_ = data_;
}

void Buffer::bzero()
{
    ::bzero(data_, size_);
}

void Buffer::append(const char *ptr, size_t len)
{
    if (left() > len)
    {
        memcpy(cur_, ptr, len);
        cur_ += len;
    }
}

void Buffer::append(AdaptedString& line)
{
    if (left() > line.length())
    {
        memcpy(cur_, line.c_str(), line.length());
        cur_ += line.length();
    }
}

const char* Buffer::data()
{
    return data_;
}

const char* Buffer::current()
{
    return cur_;
}

const char* Buffer::end()
{
    return (data_ + size_);
}

size_t Buffer::length()
{
    return static_cast<size_t>(cur_ - data_);
}