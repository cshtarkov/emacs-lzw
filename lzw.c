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

    Implementation of the LZW compression algorithm using a Trie
    with variable codeword length.

*/

#include "lzw.h"
#include "trie.h"
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 512

unsigned int compress(const char *src, unsigned int len, codeword *dest) {
    // Construct initial dictionary.
    Trie          *dict = trie_init();
    unsigned char  c    = 0;    // Will iterate from 0 to 255
    char           char_buf[2]; // String of the form "%c\0".

    char_buf[1] = '\0';
    do {
        char_buf[0] = c;
        trie_put(dict, char_buf);
        c++;
    } while (c != 0);

    // LZW.
    char         substr[BUF_SIZE];
    char         substr_char[BUF_SIZE]; // substr+ch
    unsigned int di = 0;                // Index in *dest

    strcpy(substr, "");
    for(unsigned int i = 0; i < len; i++) {
        char_buf[0] = src[i];
        strcpy(substr_char, substr);
        strcat(substr_char, char_buf);
        if(trie_get(dict, substr_char) != 0) {
            strcpy(substr, substr_char);
        } else {
            dest[di] = trie_get(dict, substr);
            di++;
            trie_put(dict, substr_char);
            strcpy(substr, char_buf);
        }
    }
    dest[di] = trie_get(dict, substr);
    di++;
    return di;
}

void decompress(const codeword *src, unsigned int len, char *dest) {
    // Construct initial dictionary.
    unsigned int   dict_size = 256+1;
    unsigned int   dict_next = 256+1;
    // An array of strings.
    char         **dict      = malloc(sizeof(char*) * dict_size); 
    // This time it's a short, because it iterates 1-256.
    short          c;           
    char           char_buf[2];

    char_buf[1] = '\0';
    c = 1;
    do {
        char_buf[0] = c-1;
        dict[c] = malloc(sizeof(char) * 2);
        strcpy(dict[c], char_buf);
        c++;
    } while (c <= 256);

    // LZW Decompression.
    codeword      cw;
    codeword      cw_prev;
    char         *dict_entry;
    char          substr_ch[BUF_SIZE];
    unsigned int  i = 0;

    strcpy(substr_ch, "");
    cw_prev = src[i];
    i++;
    strcpy(dest, dict[cw_prev]);
    while (i < len) {
        cw = src[i];
        dict_entry = dict[cw];
        strcat(dest, dict_entry);
        char_buf[0] = dict_entry[0];
        strcpy(substr_ch, dict[cw_prev]);
        strcat(substr_ch, char_buf);
        if(dict_next == dict_size) {
            dict_size *= 2;
            dict = realloc(dict, sizeof(char*) * dict_size);
        }
        dict[dict_next] = malloc(sizeof(char) * (strlen(substr_ch)+1));
        strcpy(dict[dict_next], substr_ch);
        dict_next++;
        cw_prev = cw;
        i++;
    }
}
