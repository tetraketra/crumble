#ifndef C_COMMON_H
#define C_COMMON_H

#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <libgen.h>
#include <limits.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include "types.h"
#include "log.h"

#if defined(NDEBUG) || defined(__SANITIZE_ADDRESS__)
    #define DEBUG_MODE 1
#else
    #define DEBUG_MODE 0
#endif

#define _expand(x) x

#if DEBUG_MODE
/* 
    Assert a condition at runtime. 
    If the condition is `false`, log at `LOG_FATAL` and crash.

    @param stream=stderr Optional stream to log to (if the assertion fails).
    @param cond Boolean assertion to make.
*/
    #define ASSERT_RUNTIME(...) _expand(_dmacro12(__VA_ARGS__, _asrt2, _asrt1)(__VA_ARGS__))
    #ifndef _dmacro12
        #define _dmacro12(_1, _2, name, ...) name
    #endif
    #define _asrt1(cond) do { if (!(cond)) { LOG(stderr, LOG_FATAL, "Assertion `%s` failed!", #cond); exit(1); } } while (0)
    #define _asrt2(stream, cond) do { if (!(cond)) { LOG(stream, LOG_FATAL, "Assertion `%s` failed!", #cond); exit(1); } } while (0)
#else 
/* 
    Assert a condition at runtime. 
    If the condition is `false`, log at `LOG_FATAL` and crash.

    @param stream=stderr Optional stream to log to (if the assertion fails).
    @param cond Boolean assertion to make.
*/
    #define ASSERT_RUNTIME(...) ((void)0)
#endif

/* 
    Assert a condition at compiletime. 
    If the condition is `false`, fails to compile.

    @param cond Boolean assertion to make (must be known at compiletime).
*/
#define ASSERT_STATIC(cond)  static_assert(cond)

/*
    Free a heap-allocated object, then set its pointer to `NULL`.

    @param free_fn=free Optional freeing function to use.
    @param ptr The pointer to free.

    @note If `free_fn` is specified, but is `NULL`, reverts to `free`. 
*/
#define FREE(...) _expand(_dmacro12(__VA_ARGS__, _fr2, _fr1)(__VA_ARGS__))
#ifndef _dmacro12
    #define _dmacro12(_1, _2, name, ...) name
#endif
#define _fr1(ptr)          do { if (ptr != NULL) {free(ptr); ptr = NULL;} } while (0)
#define _fr2(free_fn, ptr) do { if (ptr != NULL) {(free_fn != NULL ? (void (*)(void *))free_fn : free)(ptr); ptr = NULL;} } while (0)

/*
    The min value for `x: numeric`'s type, or `0` if not listed.

    @param x The variable whose type to get the min value for.
*/
#define MIN_VALUE_OF(x) _Generic((x), \
    char: CHAR_MIN, \
    signed char: SCHAR_MIN, \
    unsigned char: 0, \
    short: SHRT_MIN, \
    unsigned short: 0, \
    int: INT_MIN, \
    unsigned int: 0, \
    long: LONG_MIN, \
    unsigned long: 0, \
    long long: LLONG_MIN, \
    unsigned long long: 0, \
    default: ({ LOG(stderr, LOG_ERROR, "There is no `*_MIN` for this type."); 0; }) \
)

/*
    The max value for `x: numeric`'s type, or `0` if not listed.

    @param x The variable whose type to get the max value for.
*/
#define MAX_VALUE_OF(x) _Generic((x), \
    char: CHAR_MAX, \
    signed char: SCHAR_MAX, \
    unsigned char: UCHAR_MAX, \
    short: SHRT_MAX, \
    unsigned short: USHRT_MAX, \
    int: INT_MAX, \
    unsigned int: UINT_MAX, \
    long: LONG_MAX, \
    unsigned long: ULONG_MAX, \
    long long: LLONG_MAX, \
    unsigned long long: ULLONG_MAX, \
    default: ({ LOG(stderr, LOG_ERROR, "There is no `*_MAX` for this type."); 0; }) \
)

/* 
    Returns the max value of `a` and `b`.

    @param a Any type where `>` is defined.
    @param b ↑↑
*/
#define MAX(a, b) ({ typeof(a) _a = (a); typeof(b) _b = (b); (_a > _b) ? _a : _b; })

/* 
    Returns the min value of `a` and `b`.
    
    @param a Any type where `<` is defined.
    @param b ↑↑
*/
#define MIN(a, b) ({ typeof(a) _a = (a); typeof(b) _b = (b); (_a < _b) ? _a : _b; })

/* 
    Returns the absolute distance between `a` and `b`.

    @param a Any type where `>` and `-` are defined.
    @param b ↑↑
*/
#define DIST(a, b) ({ typeof(a) _a = (a); typeof(b) _b = (b); (_a > b) ? (_a - _b) : (_b - _a); })

/*
    Return `x`, clamped to `min` and/or `max`.

    @param x Any type where `<` and `>` are defined.
    @param min ↑↑
    @param max ↑↑
*/
#define CLAMP(x, min, max) ({ typeof(min) _min = (min); typeof(max) _max = (max); typeof(x) _x = (x); (_x < _min) ? _min : ((_x > _max) ? _max : _x); })

/*
    Cast `x` to `void` to avoid `Wunused`.

    @param x The value to ignore.
*/
#define IGNORE(x) ((void)(x))

/* 
    Force cast `x` to `type` with no actual conversion; just claim it's a `type`.

    @param x The value to cast.
    @param type The type to cast to.

    @note You can do this with unions, but this is shorter.
*/
#define LITERAL_CAST(x, type) ( *(type*)&val )

#endif