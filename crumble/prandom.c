#include "prandom.h"

struct prandom {
	u64 _s[4]; /* The internal random state. */
};

usize prandom_size = sizeof(prandom);

/* `xoshiro256++ 1.0` rotater function (used in `prandom_next`). */
static inline u64 _rotl(const u64 x, i8 k) {
	return (x << k) | (x >> (64 - k));
}

/* `rand()` randomization to seed `_smix64()`. */
void __attribute__((constructor)) _randseed() {
	srand(time(NULL));
	rand();
}

/* `splitmix64` generator for random `_s` seed(s). */
static u64 _smix64_x = 123;
static inline u64 _smix64(void) {
	u64 t = (_smix64_x += UINT64_C(0x9E3779B97F4A7C15));
	t = (t ^ (t >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
	t = (t ^ (t >> 27)) * UINT64_C(0x94D049BB133111EB);
	return t ^ (t >> 31);
}

/* Randomly-seeded generator, */
static prandom* _prandom_new_rseed(void) {
	prandom* pr = malloc(sizeof(prandom));
	prandom_rseed(pr);

	return pr;
}

/* Set-seed generator, */
static prandom* _prandom_new_seed(u64 seed) {
	prandom* pr = malloc(sizeof(prandom));;
	prandom_seed(pr, seed);

	return pr;
}

/* Name mangling for library simplicity. */
prandom* _prngn0(void) {
	return _prandom_new_rseed();
}

/* Name mangling for library simplicity. */
prandom* _prngn1(u64 seed) {
	return _prandom_new_seed(seed);
}


/*
	Free a pRNG generator.
*/
void prandom_free(prandom* pr) {
	ASSERT_RUNTIME(pr != NULL);

	free(pr);
}


/*
    Get pseudo-random `u64` value via xoshiro256++ 1.0.

    @note see: https://prng.di.unimi.it/xoshiro256plusplus.c
*/
u64 prandom_next(prandom* pr) {
	ASSERT_RUNTIME(pr != NULL);

	const u64 result = _rotl(pr->_s[0] + pr->_s[3], 23) + pr->_s[0];
	const u64 t = pr->_s[1] << 17;

	pr->_s[2] ^= pr->_s[0];
	pr->_s[3] ^= pr->_s[1];
	pr->_s[1] ^= pr->_s[2];
	pr->_s[0] ^= pr->_s[3];
	pr->_s[2] ^= t;
	pr->_s[3] = _rotl(pr->_s[3], 45);

	return result;
}

/*
	Seed using `seed: u64` then return the next `u64` value.

	@param seed The seed to use.

	@note The returned value is the *next* value, not the seed.
*/
u64 prandom_seed(prandom* pr, u64 seed) {
	ASSERT_RUNTIME(pr != NULL);

	seed = ((seed << 45) | (seed >> 19)) * UINT64_C(0x94D049BB133111EB);
	seed ^= ((seed >> 10) | (seed << 4)) * UINT64_C(0xBF58476D1CE4E5B9);

	pr->_s[0] = seed;
	pr->_s[1] = seed;
	pr->_s[2] = seed;
	pr->_s[3] = seed;

	return prandom_next(pr);
}

/*
	Randomly seed via `splitmix64(x = rand())` then return the next `u64` value.

	@note see: https://github.com/svaarala/duktape/blob/master/misc/splitmix64.c
*/
u64 prandom_rseed(prandom* pr) {
	ASSERT_RUNTIME(pr != NULL);
	
	pr->_s[0] = _smix64();
	pr->_s[1] = _smix64();
	pr->_s[2] = _smix64();
	pr->_s[3] = _smix64();

	return prandom_next(pr);
}
