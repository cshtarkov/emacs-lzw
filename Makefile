CC=clang
DEBUG=-g
CFLAGS=-W -Wall -Werror $(DEBUG)

default: all

all: test

test: trie_test lzw_test
	./trie_test
	./lzw_test

trie.o:
	$(CC) $(CFLAGS) trie.c -c -o trie.o

lzw.o:
	$(CC) $(CFLAGS) lzw.c -c -o lzw.o

trie_test: trie.o
	$(CC) $(CFLAGS) trie_test.c trie.o -o trie_test

lzw_test: lzw.o trie.o
	$(CC) $(CFLAGS) lzw_test.c lzw.o trie.o -o lzw_test

clean:
	rm *.o
