#!/bin/bash
ANAME="test"

# [fix bugs]
sudo sysctl vm.mmap_rnd_bits=28 > /dev/null 2>&1
sudo sysctl vm.mmap_rnd_compat_bits=8 > /dev/null 2>&1

# [build]
FILES=$(find . -print | grep -i "\.c$" | tr -s '\n' ' ')
WARNS="-W -Wall -Wextra -Wno-multichar -Wno-comment -Wno-misleading-indentation -Wno-uninitialized -Wno-unused-variable"
FSANS="-fsanitize=address -fsanitize=undefined -fsanitize-address-use-after-scope" # FOR RELEASE, CLEAR THIS AND REMOVE THE DLCLOSE EXPORT
LINKS=""
FLAGS="-Og -g3"
INCLD="-iquote ./crumble"
BUILD_START=$(date +%s.%N)
mkdir -p ./build
gcc-12 $FILES -o ./build/$ANAME \
    $WARNS $LINKS $FLAGS $FSANS $INCLD
BUILD_END=$(date +%s.%N)
find ./build -type f -print -quit | xargs chmod a+x

# [report]
echo "Built in $(echo "$BUILD_END $BUILD_START - p" | dc | xargs printf "%.2f")s."
echo "Execute \"sh run.sh\" to start the program."