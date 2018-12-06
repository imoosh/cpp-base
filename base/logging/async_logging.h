//
// Created by root on 11/29/18.
//

#ifndef CODEBASE_ASYNC_LOGGING_H
#define CODEBASE_ASYNC_LOGGING_H

#include <base/types/types.h>
#include <base/buffer/buffer.h>
#include <base/thread/mutexlock.h>
#include <base/thread/thread.h>
#include <base/string/adapted_string.h>

#include <boost/bind.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

class AsyncLogging : boost::noncopyable
{
public:
    /*
     * @path：日志存放路径
     * @basename：日志文件基名
     * @roll_size：刷新文件缓存大小
     * @flush_interval：刷新日志文件时间间隔
     */
    explicit AsyncLogging(AdaptedString logging_path_=".", AdaptedString logging_basename_="", int roll_size=64, int flush_interval=3);
    ~AsyncLogging() = default;

    void start();
    void stop();

    void append(AdaptedString line);

    // 防止编译器合成拷贝构造函数和拷贝赋值运算符，BufferPtr不允许直接赋值
    AsyncLogging(const AsyncLogging&) = delete;
    AsyncLogging& operator=(const AsyncLogging&) = delete;

private:
    // 同上，只申明不定义亦可
    // Logging(const Logging&);
    // Logging& operator=(const Logging&);

    typedef boost::ptr_vector<Buffer> BufferVector;
    typedef BufferVector::auto_type BufferPtr;

    void loggingFunc();

    const string logging_path_;
    const string logging_basename_;
    int roll_size_;
    int flush_interval_;
    bool running_;
    Thread thread_;
    MutexLock lock_;
    CondVariable cond_;

    BufferPtr current_buffer_;
    BufferVector full_buffers_;
    BufferVector empty_buffers_;

    // 单个文件最大值
    static const size_t kLoggingBufferSize = (4 << 20);
    static const size_t kEmptyBuffersInitSize = 4;
};

#endif //CODEBASE_ASYNC_LOGGING_H
