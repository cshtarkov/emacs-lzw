/**
   Copyright © 2016 Christian Shtarkov

   This file is part of emacs-lzw.

   emacs-lzw is free software: you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation, either version 3 of the License, or (at your
   option) any later version.

   emacs-lzw is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with emacs-lzw. If not, see http://www.gnu.org/licenses/.

   Commentary:

   Header file for the trie. See trie.c for implementation details.

*/

#ifndef TRIE_H
#define TRIE_H

#ifndef NULL
#define NULL 0
#endif

typedef struct trie_child TrieChild;
typedef struct trie_node TrieNode;
typedef struct trie Trie;
typedef unsigned int codeword;

Trie*    trie_init    ();
void     trie_put     (Trie *t, const char *w);
codeword trie_get     (Trie *t, const char *w);
void     trie_destroy (Trie *t);

#endif
