CC := "clang"
CFLAGS := "-Wall -Wextra -std=c99"


build_test:
    {{ CC }} {{ CFLAGS }} tests/tests.c -o tests.out

test: build_test
    ./tests.out

clean:
    rm tests.out