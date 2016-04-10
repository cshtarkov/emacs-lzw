CC=clang
DEBUG=-g
CFLAGS=-W -Wall -Werror -fPIC $(DEBUG)
SRC_DIR=src
TESTS_DIR=tests
MODULES_DIR=~/.emacs.d/modules

default: all

all: test emacs_lzw.so

test: trie_test lzw_test
	./trie_test
	./lzw_test

trie.o:
	$(CC) $(CFLAGS) $(SRC_DIR)/trie.c -c -o trie.o

lzw.o:
	$(CC) $(CFLAGS) $(SRC_DIR)/lzw.c -c -o lzw.o

io.o:
	$(CC) $(CFLAGS) $(SRC_DIR)/io.c -c -o io.o

emacs_lzw.so: emacs_lzw.o lzw.o io.o trie.o
	$(CC) -shared emacs_lzw.o lzw.o io.o trie.o -o emacs_lzw.so

emacs_lzw.o:
	$(CC) $(CFLAGS) $(SRC_DIR)/emacs_lzw.c -c -o emacs_lzw.o

trie_test: trie.o
	$(CC) $(TESTS_DIR)/trie_test.c trie.o -o trie_test

lzw_test: lzw.o trie.o
	$(CC) $(TESTS_DIR)/lzw_test.c lzw.o trie.o -o lzw_test

clean:
	rm -f *.o *.so

install: emacs_lzw.so
	mkdir -p $(MODULES_DIR)
	cp emacs_lzw.so $(MODULES_DIR)
