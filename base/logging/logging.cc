//
// Created by win on 11/23/18.
//

#include "base/logging/logging.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <boost/bind.hpp>

#include <base/types/types.h>
#include <base/buffer/buffer.h>

Logging::Logging(const string& logging_path, const string& basename, int roll_size, int flush_interval) :
    logging_path_(logging_path),
    basename_(basename),
    roll_size_(roll_size),
    flush_interval_(flush_interval),
    running_(false),
    thread_(boost::bind(&Logging::loggingFunc, this), "LoggingThread"),
    lock_(),
    cond_(lock_),
    buffers_(),
    current_buffer_(new Buffer(kLoggingBufferSize)),
    next_buffer_(new Buffer(kLoggingBufferSize))
{
    current_buffer_->bzero();
    next_buffer_->bzero();
    buffers_.reserve(16);
}

void Logging::append(const char *line, size_t len)
{
    ScopedLock lock(lock_);
    if (current_buffer_->left() > len)
    {
        // 写入缓存
        current_buffer_->append(line, static_cast<size_t>(len));
    }
    else
    {
        // current_buffer_放入buffers_中
        buffers_.push_back(current_buffer_.release());

        if (next_buffer_)
        {
            // 如果next_buffer_可用，将next_buffer_交换到current_buffer_
            current_buffer_ = boost::ptr_container::move(next_buffer_);
        }
        else
        {
            // 如果next_buffer_不可用，新分配一块buffer交换到current_buffer_
            // 不可用的原因：后台刷新文件的线程没来得及处理缓冲区。交换回空闲的缓冲区。
            current_buffer_.reset(new Buffer(kLoggingBufferSize));
        }

        // 最后将当前日志写入当前缓冲区，并通知后台线程处理已写满的日志缓冲区
        current_buffer_->append(line, static_cast<size_t>(len));
        cond_.notify();
    }
}

void Logging::loggingFunc()
{
    ASSERT(!running_);

    BufferVector write_buffers;
    BufferPtr buffer1(new Buffer(kLoggingBufferSize));
    BufferPtr buffer2(new Buffer(kLoggingBufferSize));
    buffer1->bzero();
    buffer2->bzero();

    while (running_)
    {
        {
            ScopedLock lock(lock_);
            if (buffers_.empty())
            {
                cond_.waitForSeconds(flush_interval_);
            }

            ASSERT(!buffers_.empty());

            buffers_.push_back(current_buffer_.release());
            current_buffer_ = boost::ptr_container::move(buffer1);

            write_buffers.swap(buffers_);

            if (!next_buffer_)
            {
                next_buffer_ = boost::ptr_container::move(buffer2);
            }
        }

        ASSERT(!write_buffers.empty());

        if (write_buffers.size() > 16)
        {
            fprintf(stderr, "WARNING: WriteBuffers is too large.\n");
            write_buffers.erase(write_buffers.begin()+2, write_buffers.end());
        }

        for (BufferVector::size_type i = 0; i < write_buffers.size(); i++)
        {
            // append file
        }

        if (write_buffers.size() > 2)
        {
            Buffer buffer(kLoggingBufferSize);
            write_buffers.resize(2, &buffer);
        }

        if (!buffer1)
        {
            buffer1 = write_buffers.pop_back();
        }

        if (!buffer2)
        {
            buffer2 = write_buffers.pop_back();
        }

        write_buffers.clear();

        // flush file
    }
    // flush file
}

void Logging::start()
{
    running_ = true;
    thread_.start();
}

void Logging::stop()
{
    running_ = false;
    cond_.notify();
    thread_.join();
}