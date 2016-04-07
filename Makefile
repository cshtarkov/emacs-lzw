CC=clang
CFLAGS=-W -Wall -Werror

default: all

all: test

test: trie_test
	./trie_test

trie.o:
	$(CC) $(CFLAGS) trie.c -c -o trie.o

lzw.o:
	$(CC) $(CFLAGS) lzw.c -c -o lzw.o

trie_test: trie.o
	$(CC) $(CFLAGS) trie_test.c trie.o -o trie_test

clean:
	rm *.o
