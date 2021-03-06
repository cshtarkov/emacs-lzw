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

    Tests for the Trie implementation.
    This is not a general-purpose implementation, it's meant
    to be used with the LZW algorithm.
    For relevant design decisions, see trie.c

*/

#include <stdio.h>
#include <assert.h>
#include "../src/trie.h"

void test_init() {
    Trie *t = trie_init();
    assert(t != NULL);
}

#define INSERTIONS(t)                           \
    trie_put(t, "a", 1);                        \
    trie_put(t, "b", 1);                        \
    trie_put(t, "c", 1);                        \
    trie_put(t, "ab", 2);                       \
    trie_put(t, "ab", 2);                       \
    trie_put(t, "ab", 2);                       \
    trie_put(t, "aa", 2);                       \
    trie_put(t, "ab", 2);                       \
    trie_put(t, "ba", 2);                       \
    trie_put(t, "bb", 2);                       \
    trie_put(t, "ca", 2);                       \
    trie_put(t, "cc", 2);                       \
    trie_put(t, "aaa", 3);

void test_put() {
    Trie *t = trie_init();
    INSERTIONS(t);
}

void test_get() {
    Trie *t = trie_init();
    INSERTIONS(t);
    assert(trie_get(t, "a", 1) == 1);
    assert(trie_get(t, "b", 1) == 2);
    assert(trie_get(t, "c", 1) == 3);
    assert(trie_get(t, "ca", 2) == 8);
    assert(trie_get(t, "aaa", 3) == 10);
}

int main(void) {
    test_init();
    test_put();
    test_get();
    return 0;
}
