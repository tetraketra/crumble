#!/bin/bash
ANAME="test"

# [fix bugs]
export LD_PRELOAD="/usr/lib/gcc/x86_64-linux-gnu/12/libasan.so ./build/dlclose.so" # FOR RELEASE, COMMENT THIS AND CLEAR THE FSANS
export ASAN_OPTIONS=detect_leaks=1:halt_on_error=0:quarantine_size_mb=128

# [report]
./build/$ANAME