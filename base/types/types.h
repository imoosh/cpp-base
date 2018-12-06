//
// Created by win on 11/7/18.
//

#ifndef BASE_TYPES_H
#define BASE_TYPES_H

#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <iostream>

using namespace std;

#ifdef BASE_STD_STRING
#include <string>
#else // !BASE_STD_STRING
#include <ext/vstring.h>
#include <ext/vstring_fwd.h>
#endif

#ifdef BASE_STD_STRING
using std::string;
#else // !BASE_STD_STRING
typedef __gnu_cxx::__sso_string string;
#endif

#ifdef NDEBUG
#define ASSERT(expr)            __ASSERT_VOID_CAST(expr)
#define ASSERT_ERROR(errnum)    __ASSERT_VOID_CAST(expr)
#else // !NDEBUG
#define ASSERT(expr)            assert(expr)
#define ASSERT_PERROR(errnum)   assert_perror(errnum)
#endif

#define LIKELY(x)           __builtin_expect(!!(x), 1)      /* general condition */
#define UNLIKELY(x)         __builtin_expect(!!(x), 0)      /* rarely condition */

extern __thread char t_errorbuf[512];

extern const char *strerror_tl(int errnum);

#endif //BASE_TYPES_H