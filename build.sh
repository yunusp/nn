#! /bin/sh

set -xe

clang -Wall -Wextra -o gates gates.c -lm
clang -Wall -Wextra -o main main.c -lm
clang -Wall -Wextra -o xor xor.c -lm
