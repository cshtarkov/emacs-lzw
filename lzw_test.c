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

    Tests for the LZW compression algorithm.

*/

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "lzw.h"

void test_compress() {
    const char src[] = "hello_world_hello_hello_hello";
    unsigned int len = strlen(src);
    codeword dest[256];
    unsigned int dlen = compress(src, len, dest);
    printf("Source string: %s\n", src);
    printf("Compressed:\n");
    for(unsigned int i = 0; i < dlen; i++) {
        printf("%d ", dest[i]);
    }
    printf("\n");
}

int main(void) {
    test_compress();
    return 0;
}
