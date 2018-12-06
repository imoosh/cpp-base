//
// Created by win on 11/23/18.
//

#include <base/types/types.h>

#if 1
__thread char t_errorbuf[512];

const char *strerror_tl(int errnum)
{
    return strerror_r(errnum, t_errorbuf, sizeof(t_errorbuf));
}
#endif