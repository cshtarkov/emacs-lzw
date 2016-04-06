CC=clang
CFLAGS=-W -Wall -Werror

trie.o:
	$(CC) $(CFLAGS) trie.c -o trie.o
