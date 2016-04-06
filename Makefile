CC=clang
CFLAGS=-W -Wall -Werror

trie.o:
	$(CC) $(CFLAGS) trie.c -c -o trie.o

clean:
	rm *.o
