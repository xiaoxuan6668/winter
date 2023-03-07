#ifndef __WINTER_MACRO_H__
#define __WINTER_MACRO_H__

#include <string.h>
#include <assert.h>
#include "util.h"

#define WINTER_ASSERT(x) \
    if(!(x)) { \
        WINTER_LOG_ERROR(WINTER_LOG_ROOT()) << "ASSERTION: " #x \
            << "\nbacktrace:\n" \
            << winter::BacktraceToString(100, 2, "      "); \
        assert(x); \
    }

#define WINTER_ASSERT2(x, w) \
    if(!(x)) { \
        WINTER_LOG_ERROR(WINTER_LOG_ROOT()) << "ASSERTION: " #x \
            << "\n" << w \
            << "\nbacktrace:\n" \
            << winter::BacktraceToString(100, 2, "      "); \
        assert(x); \
    }

#endif
