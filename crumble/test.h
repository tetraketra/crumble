#ifndef C_TEST_H
#define C_TERT_H

#include <sanitizer/lsan_interface.h>
#include <sanitizer/asan_interface.h>
#include <unistd.h>
#include <fcntl.h>

#include "common.h"
#include "prandom.h"

extern bool test_crumble(void);

#endif