# A Makefile to build the brz_util C library
CC=gcc
CFLAGS=-Wall -pedantic -std=c99

# The code layout is currently simple enough to justify just wildcarding
SRC=$(wildcard *.c)
TEST_SRC=$(wildcard tests/*.c)
HEADERS=$(wildcard *.h)
TEST_HEADERS=$(wildcard tests/*.h)

test: $(SRC) $(TEST_SRC) $(HEADERS) $(TEST_HEADERS)
	$(CC) $(CFLAGS) -o test $(SRC) $(TEST_SRC)
	
clean:
	rm -f test
