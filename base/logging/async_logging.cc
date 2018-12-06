//
// Created by root on 11/29/18.
//

#include <base/logging/async_logging.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <boost/bind.hpp>

#include <base/types/types.h>
#include <base/buffer/buffer.h>
#include <base/logging/logging_file.h>

AsyncLogging::AsyncLogging(AdaptedString logging_path, AdaptedString logging_basename, int roll_size, int flush_interval) :
        logging_path_(logging_path.c_str()),
        logging_basename_(logging_basename.c_str()),
        roll_size_(roll_size),
        flush_interval_(flush_interval),
        running_(false),
        thread_(boost::bind(&AsyncLogging::loggingFunc, this), "LoggingThread"),
        lock_(),
        cond_(lock_),
        current_buffer_(),
        full_buffers_(),
        empty_buffers_()
{
    full_buffers_.reserve(16);
    empty_buffers_.reserve(16);

    /* 初始化若干个缓冲区到空闲区 */
    for (BufferVector::size_type i = 0; i < kEmptyBuffersInitSize; i++)
    {
        BufferPtr buffer_ptr(new Buffer(kLoggingBufferSize));
        empty_buffers_.push_back(buffer_ptr.release());
    }

    /* 将第一个缓冲区交换到current_buffer_上 */
    current_buffer_ = empty_buffers_.release(empty_buffers_.begin());
    current_buffer_->bzero();
}

void AsyncLogging::append(AdaptedString line)
{
    ScopedLock lock(lock_);

    if (current_buffer_->left() > line.length())
    {
        // 写入缓存
        current_buffer_->append(line);
    }
    else
    {
        // current_buffer_缓冲区放入buffers_中
        full_buffers_.push_back(current_buffer_.release());

        if (!empty_buffers_.empty())
        {
            // 如果有空闲缓冲区，取一个交换到current_buffer_
            current_buffer_ = empty_buffers_.release(empty_buffers_.begin());
            current_buffer_->bzero();
        }
        else
        {
#if 0
            // 如果没有空闲缓冲区，新分配一块直接交换到current_buffer_
            // 不可用的原因：后台刷新文件的线程没来得及处理缓冲区。交换回空闲的缓冲区。
            current_buffer_.reset(new Buffer(kLoggingBufferSize));
#else
            // 将full_buffers_中最旧的待写缓冲区交换到current_buffer_
            fprintf(stderr, "WARNING: logging buffers has been used up.\n");
            current_buffer_ = full_buffers_.release(full_buffers_.begin());
#endif
        }

        // 最后将当前日志写入当前缓冲区，并通知后台线程处理已写满的日志缓冲区
        current_buffer_->append(line);
        cond_.notify();
    }
}

void AsyncLogging::loggingFunc()
{
    ASSERT(running_);
    BufferVector write_buffers;
    LoggingFile logging_file(logging_path_, logging_basename_);

    while (running_)
    {
        {
            ScopedLock lock(lock_);
            if (full_buffers_.empty())
            {
                // 如果full_buffers_为空，进入休眠状态
                cond_.waitForSeconds(flush_interval_);
            }

#if 0
            // 刷新时间到，无论current_buffer_有多少日志数据，都交换过来写入文件
            full_buffers_.push_back(current_buffer_.release());
            current_buffer_ = empty_buffers_.release(empty_buffers_.begin());
            if (!current_buffer_)
            {
                // empty_buffers用完，丢弃full_buffers_中最旧的缓冲区，交换到current_buffer_上
                fprintf(stderr, "Discard the oldest full logging buffer!\n");
                //current_buffer_ = full_buffers_.release(full_buffers_.begin());
            }
#endif
            // 将full_buffers_交换到write_buffers上
            write_buffers.swap(full_buffers_);
        }

        BufferVector::size_type buffers_size = write_buffers.size();
        for (BufferVector::size_type i = 0; i < buffers_size; i++)
        {
            // append file
            BufferPtr buffer = write_buffers.release(write_buffers.begin());
            logging_file.append(buffer->data(), buffer->length());

            buffer->reset();
            buffer->bzero();

            {
                // buffer写文件后，放入空闲缓冲区组中
                ScopedLock lock(lock_);
                empty_buffers_.push_back(buffer.release());
            }
        }

        // flush file
        logging_file.flush();
    }

    // flush file
    logging_file.flush();
}

void AsyncLogging::start()
{
    running_ = true;
    thread_.start();
}

void AsyncLogging::stop()
{
    running_ = false;
    cond_.notify();
    thread_.join();
}
