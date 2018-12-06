//
// Created by win on 11/7/18.
//

#ifndef BASE_LOGGER_H
#define BASE_LOGGER_H

#include <string.h>
#include <stddef.h>

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#include <base/types/types.h>
#include <base/thread/mutexlock.h>
#include <base/logging/async_logging.h>
#include <base/singleton/singleton.h>

template<typename T>
class Logger : boost::noncopyable
{
public:
    // C++11单例模式实现构造时参数传入(可变参数的模板函数)
    template <typename... Args>
    static T* getLogger(Args&& ...args)
    {
        return Singleton<T>::getInstance(args...);
    }
};

#endif //BASE_LOGGER_H
