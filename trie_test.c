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
#include "trie.h"

void test_init() {
    Trie *t = trie_init();
    assert(t != NULL);
}

int main(void) {
    test_init();
    return 0;
}
