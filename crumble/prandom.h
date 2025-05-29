#ifndef C_PRANDOM_H
#define C_PRANDOM_H

#include <stdint.h>
#include <time.h>

#include "common.h"

typedef struct prandom prandom;
extern usize prandom_size; /* Total size, including internals. */

/*
	Create a new pRNG generator on the heap.

    @param seed=prandom_rseed() Optional seed to initialize with. 
*/
#define prandom_new(...) _dmacro01(_0, ##__VA_ARGS__, _prngn1, _prngn0)(__VA_ARGS__)
#ifndef _dmacro01
    #define _dmacro01(_0, _1, name, ...) name
#endif
prandom* _prngn0(void);
prandom* _prngn1(u64 seed);

extern u64 prandom_next(prandom* pr);
extern u64 prandom_seed(prandom* pr, u64 seed);
extern u64 prandom_rseed(prandom* pr);
extern void prandom_free(prandom* pr);

/*
    Get pseudo-random `float` value via xoshiro256++ 1.0.

    @param pr The `prandom` object to use.
    @param min=0.0f Optional inclusive floor. Cannot be used alone.
    @param max=1.0f Optional inclusive ceiling. Can be used alone or with `min`.

    @note see: https://prng.di.unimi.it/xoshiro256plusplus.c
    @note Has 3 overloads: `f()`, `f(max)`, and `f(min, max)`.
    @note Only specifying `f(min)` will overload to `f(max)`.
*/
#define prandom_next_float(...) _expand(_dmacro123(__VA_ARGS__, _prngnf3, _prngnf2, _prngnf1)(__VA_ARGS__))
#ifndef _dmacro123
    #define _dmacro123(_1, _2, _3, name, ...) name
#endif
#define _prngnf1(pr) ((float)prandom_next(pr)/(float)UINT64_MAX)
#define _prngnf2(pr, max) (_prngnf1(pr)*(max))
#define _prngnf3(pr, min, max) ({ typeof(min) _min = (min); _prngnf1(pr) * ((max) - _min) + _min; })

#endif