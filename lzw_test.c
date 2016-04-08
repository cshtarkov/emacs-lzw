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
    const char src[] = "hello_world_hello_hello";
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

void test_decompress() {
    const codeword src[] = {104, 101, 108, 108, 111, 95,
                            119, 111, 114, 108, 100, 95,
                            256, 258, 260, 268, 259};
    unsigned int len = 17;
    char dest[256];
    printf("Source code: \n");
    for(unsigned int i = 0; i < len; i++) {
        printf("%d ", src[i]);
    }
    decompress(src, len, dest);
    printf("Decompressed: %s\n", dest);
}

void test_reversibility() {
    const char src[] = "some_compressed_string_some_string_compressed";
    char dsrc[256];
    unsigned int len = strlen(src);
    codeword dest[256];
    unsigned int dlen = compress(src, len, dest);
    decompress(dest, dlen, dsrc);
    assert(!strcmp(src, dsrc));
}

int main(void) {
    test_compress();
    test_decompress();
    test_reversibility();
    return 0;
}
