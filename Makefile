# A Makefile to build the brz_util C library
CC=gcc
CFLAGS=-Wall -pedantic -std=c99

# The code layout is currently simple enough to justify just wildcarding
INCLUDE_DIR='/usr/local/include/'
LIB_DIR='/usr/local/lib/'

SRC=$(wildcard *.c)
TEST_SRC=$(wildcard tests/*.c)
HEADERS=$(wildcard *.h)
TEST_HEADERS=$(wildcard tests/*.h)

SRC_DIR=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

test: $(SRC) $(TEST_SRC) $(HEADERS) $(TEST_HEADERS)
	$(CC) $(CFLAGS) -o test $(SRC) $(TEST_SRC)

lib: $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) -c $(SRC)
	ar -cvq out/libbrz_utils.a *.o

install: 
	make clean && make lib
	sudo chmod +x ./out/libbrz_utils.a
	sudo ln -s $(SRC_DIR)/out/libbrz_utils.a $(LIB_DIR)
	sudo ln -s $(SRC_DIR)/brz_utils.h $(INCLUDE_DIR)

uninstall:
	make clean
	sudo rm $(INCLUDE_DIR)brz_utils.h
	sudo rm $(LIB_DIR)libbrz_utils.a

clean:
	rm -f test *.o
	rm -f /out/*
