set(base_SRCS
        types/types.cc
        time/timestamp.cc
        timer/posix_timer.cc
        thread/mutexlock.cc
        thread/condvariable.cc
        thread/thread.cc
        buffer/buffer.cc
        logging/logger.cc
        logging/logging.cc
        logging/async_logging.cc
        logging/logging_file.cc
        file/append_file.cc)

add_library(base ${base_SRCS})
target_link_libraries(base pthread rt)