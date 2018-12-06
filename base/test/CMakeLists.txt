set(CXX_FLAGS "${CXX_FLAGS} -Wno-unused-variable")
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

#message("###" ${CXX_FLAGS})
#message("###" ${CMAKE_CXX_FLAGS})

set(test_apps
        timestamp_test
        posix_timer_test
        thread_test
        logger_test
        logging_file_test
        async_logging_test
        singleton_test
        )

foreach(app ${test_apps})
    add_executable(${app} ${app}.cc)
    target_link_libraries(${app} base)
endforeach()
