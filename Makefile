CC=clang
DEBUG=-g
CFLAGS=-W -Wall -Werror -fPIC $(DEBUG)

default: all

all: test emacs-lzw.so

test: trie_test lzw_test
	./trie_test
	./lzw_test

trie.o:
	$(CC) $(CFLAGS) trie.c -c -o trie.o

lzw.o:
	$(CC) $(CFLAGS) lzw.c -c -o lzw.o

io.o:
	$(CC) $(CFLAGS) io.c -c -o io.o

emacs-lzw.so: emacs-lzw.o lzw.o io.o trie.o
	$(CC) -shared emacs-lzw.o lzw.o io.o trie.o -o emacs-lzw.so

emacs-lzw.o:
	$(CC) $(CFLAGS) emacs-lzw.c -c -o emacs-lzw.o

trie_test: trie.o
	$(CC) trie_test.c trie.o -o trie_test

lzw_test: lzw.o trie.o
	$(CC) lzw_test.c lzw.o trie.o -o lzw_test

clean:
	rm *.o
