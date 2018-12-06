//
// Created by win on 11/23/18.
//

#ifndef BASE_LOGGING_H
#define BASE_LOGGING_H

#include <base/types/types.h>
#include <base/buffer/buffer.h>
#include <base/thread/mutexlock.h>
#include <base/thread/thread.h>

#include <boost/bind.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

class Logging : boost::noncopyable
{
public:
    /*
     * @path：日志存放路径
     * @basename：日志文件基名
     * @roll_size：刷新文件缓存大小
     * @flush_interval：刷新日志文件时间间隔
     */
    Logging(const string& logging_path_, const string& basename, int roll_size=64, int flush_interval=3);
    ~Logging() = default;

    void start();
    void stop();

    void append(const char *line, size_t len);

    // 防止编译器合成拷贝构造函数和拷贝赋值运算符，BufferPtr不允许直接赋值
    Logging(const Logging&) = delete;
    Logging& operator=(const Logging&) = delete;

private:
    // 同上，只申明不定义亦可
    // Logging(const Logging&);
    // Logging& operator=(const Logging&);

    typedef boost::ptr_vector<Buffer> BufferVector;
    typedef BufferVector::auto_type BufferPtr;

    void loggingFunc();

    const string logging_path_;
    const string basename_;
    int roll_size_;
    int flush_interval_;
    bool running_;
    Thread thread_;
    MutexLock lock_;
    CondVariable cond_;

    BufferVector buffers_;
    BufferPtr current_buffer_;
    BufferPtr next_buffer_;

    static const size_t kLoggingBufferSize = (4 << 20);
};

#endif //BASE_LOGGING_H
