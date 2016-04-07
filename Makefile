CC=clang
CFLAGS=-W -Wall -Werror

trie.o:
	$(CC) $(CFLAGS) trie.c -c -o trie.o

lzw.o:
	$(CC) $(CFLAGS) lzw.c -c -o lzw.o

clean:
	rm *.o
