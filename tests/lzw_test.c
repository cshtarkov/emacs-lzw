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
#include "../src/lzw.h"

void test_compress_text() {
    const char src[] = "abababababababa";
    unsigned int len = strlen(src);
    codeword dest[256];
    unsigned int dlen = lzw_compress(src, len, dest);
    printf("Source string: %s\n", src);
    printf("Compressed:\n");
    for(unsigned int i = 0; i < dlen; i++) {
        printf("%d ", dest[i]);
    }
    printf("\n");
}

void test_decompress_text() {
    const codeword src[] = {41, 105, 102, 109, 109, 112, 96, 120, 112, 115, 109, 101, 96, 257, 259, 261, 269, 260, 268, 258, 273, 272, 271, 275, 112};
    unsigned int len = 25;
    printf("Source code: \n");
    for(unsigned int i = 0; i < len; i++) {
        printf("%d ", src[i]);
    }
    printf("\n");
    decompression_meta m = lzw_decompress(src, len);
    unsigned int dlen = m.dlen;
    char *dest = m.str;
    printf("Decompressed: ");
    for(unsigned int i = 0; i < dlen; i++) {
        printf("%c", dest[i]);
    }
    printf("\n");
}

void test_compress_binary() {
    const char src[] = {15, 20, 100, 0, 255, 30, 0, 5, 15, 20, 100, 0, 30, 0, 15, 20, 100};
    unsigned int len = 17;
    codeword dest[256];
    printf("Source binary: \n");
    for(unsigned int i = 0; i < len; i++) {
        printf("%d ", src[i]);
    }
    printf("\n");
    unsigned int dlen = lzw_compress(src, len, dest);
    printf("Compressed:\n");
    for(unsigned int i = 0; i < dlen; i++) {
        printf("%d ", dest[i]);
    }
    printf("\n");
}

void test_decompress_binary() {
    const codeword src[] = {16, 21, 101, 1, 256, 31,
                            1, 6, 257, 259, 262, 265};
    unsigned int len = 12;
    printf("Source code: \n");
    for(unsigned int i = 0; i < len; i++) {
        printf("%d ", src[i]);
    }
    printf("\n");
    decompression_meta m = lzw_decompress(src, len);
    unsigned int dlen = m.dlen;
    char *dest = m.str;
    printf("Decompressed:\n");
    for(unsigned int i = 0; i < dlen; i++) {
        printf("%d ", dest[i]);
    }
    printf("\n");
}

void test_reversibility_text() {
    const char src[] = "abababababababa";
    char *dsrc;
    unsigned int len = strlen(src);
    codeword dest[256];
    unsigned int dlen = lzw_compress(src, len, dest);
    decompression_meta m = lzw_decompress(dest, dlen);
    dlen = m.dlen;
    dsrc = m.str;
    assert(len == dlen);
    for(unsigned i = 0; i < len; i++) {
        assert(src[i] == dsrc[i]);
    }
    printf("pass\n");
}

void test_reversibility_binary() {
    const char src[] = {100, 0, 15, 100, 0, 0, 20, 0, 0, 100, 100, 100, 0, 15, 100, 0, 15};
    char *dsrc;
    unsigned int len = 17;
    codeword dest[256];
    unsigned int dlen = lzw_compress(src, len, dest);
    decompression_meta m = lzw_decompress(dest, dlen);
    dlen = m.dlen;
    dsrc = m.str;
    assert(len == dlen);
    for(unsigned int i = 0; i < len; i++) {
        assert(src[i] == dsrc[i]);
    }
    printf("pass\n");
}

#define heading(s) printf("\n\n--- %s ---\n", s)

int main(void) {
    heading("text compression");
    test_compress_text();
    heading("text decompression");
    test_decompress_text();
    heading("text reversibility");
    test_reversibility_text();
    heading("binary compression");
    test_compress_binary();
    heading("binary decompression");
    test_decompress_binary();
    heading("binary reversibility");
    test_reversibility_binary();
    return 0;
}
