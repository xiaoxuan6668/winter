#ifndef __WINTER_MACRO_H__
#define __WINTER_MACRO_H__

#include <string.h>
#include <assert.h>
#include "util.h"

#if defined __GNUC__ || defined __llvm__
#   define WINTER_LICKLY(x)       __builtin_expect(!!(x), 1)
#   define WINTER_UNLICKLY(x)     __builtin_expect(!!(x), 0)
#else
#   define WINTER_LICKLY(x)      (x)
#   define WINTER_UNLICKLY(x)      (x)
#endif

#define WINTER_ASSERT(x) \
    if(WINTER_UNLICKLY(!(x))) { \
        WINTER_LOG_ERROR(WINTER_LOG_ROOT()) << "ASSERTION: " #x \
            << "\nbacktrace:\n" \
            << winter::BacktraceToString(100, 2, "    "); \
        assert(x); \
    }

#define WINTER_ASSERT2(x, w) \
    if(WINTER_UNLICKLY(!(x))) { \
        WINTER_LOG_ERROR(WINTER_LOG_ROOT()) << "ASSERTION: " #x \
            << "\n" << w \
            << "\nbacktrace:\n" \
            << winter::BacktraceToString(100, 2, "    "); \
        assert(x); \
    }

#endif