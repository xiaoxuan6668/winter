/**
 * @file macro.h
 * @brief 常用宏的封装
 */
#ifndef __WINTER_MACRO_H__
#define __WINTER_MACRO_H__

#include <string.h>
#include <assert.h>
#include "util.h"
#include "log.h"

#if defined __GNUC__ || defined __llvm__
// LIKCLY 宏的封装, 告诉编译器优化,条件大概率成立
#   define WINTER_LIKELY(x)       __builtin_expect(!!(x), 1)
// LIKCLY 宏的封装, 告诉编译器优化,条件大概率不成立
#   define WINTER_UNLIKELY(x)     __builtin_expect(!!(x), 0)
#else
#   define WINTER_LIKELY(x)      (x)
#   define WINTER_UNLIKELY(x)      (x)
#endif

// 断言宏封装
#define WINTER_ASSERT(x) \
    if(WINTER_UNLIKELY(!(x))) { \
        WINTER_LOG_ERROR(WINTER_LOG_ROOT()) << "ASSERTION: " #x \
            << "\nbacktrace:\n" \
            << winter::BacktraceToString(100, 2, "    "); \
        assert(x); \
    }

// 断言宏封装
#define WINTER_ASSERT2(x, w) \
    if(WINTER_UNLIKELY(!(x))) { \
        WINTER_LOG_ERROR(WINTER_LOG_ROOT()) << "ASSERTION: " #x \
            << "\n" << w \
            << "\nbacktrace:\n" \
            << winter::BacktraceToString(100, 2, "    "); \
        assert(x); \
    }

#endif