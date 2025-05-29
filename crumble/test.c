#include "test.h"

static void test_prandom(void) {
    prandom* pr1 = prandom_new();
    prandom_next(pr1);
    prandom_seed(pr1, 123);
    prandom_next(pr1);
    prandom_rseed(pr1);
    prandom_next(pr1);

    prandom* pr2 = prandom_new(123);
    ASSERT_RUNTIME(prandom_next_float(pr2, 0.7f, 0.8f) - 0.755701 < 0.00001);
    ASSERT_RUNTIME(prandom_next_float(pr2, 0.7f, 0.8f) - 0.766208 < 0.00001);
    ASSERT_RUNTIME(prandom_next_float(pr2, 0.7f, 0.8f) - 0.781467 < 0.00001);
    ASSERT_RUNTIME(prandom_next_float(pr2, 0.7f, 0.8f) - 0.724838 < 0.00001);
    ASSERT_RUNTIME(prandom_next_float(pr2, 0.7f, 0.8f) - 0.730469 < 0.00001);

    FREE(prandom_free, pr1);
    FREE(prandom_free, pr2);
}

static void test_log(void) {
    LOG_LEVEL(LOG_WARN);
    LOG(stderr, LOG_INFO, "This shouldn't print.");
    LOG(stderr, LOG_FATAL, "This should print (it isn't fatal).");
    LOG_LEVEL(LOG_DEBUG);
    LOG(stderr, LOG_DEBUG, "Color demo!");
    LOG(stderr, LOG_INFO, "Color demo!");
    LOG(stderr, LOG_WARN, "Color demo!");
    LOG(stderr, LOG_ERROR, "Color demo!");
    LOG(stderr, LOG_FATAL, "Color demo!");
}

bool test_crumble(void) {
    LOG(stderr, LOG_INFO, "Beginning test(s). Passing tests does not guarantee no leaks.");

    /* prandom.c|h */
    LOG(stderr, LOG_INFO, "`prandom.c|h` test(s) started.");
    test_prandom();
    LOG(stderr, LOG_INFO, "`prandom.c|h` test(s) finished.");

    /* log.c|h */
    LOG(stderr, LOG_INFO, "`log.c|h` test(s) started.");
    test_log();
    LOG(stderr, LOG_INFO, "`log.c|h` test(s) finished.");

    bool any_leaks = __lsan_do_recoverable_leak_check();
    if (any_leaks) {
        LOG(stderr, LOG_ERROR, "Test(s) leaked!");
    }

    LOG(stderr, LOG_INFO, "Ending test(s). All mid-content asan leaks will now be repeated.");
    return any_leaks;
}