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

    Implementation of the LZW compression algorithm using a Trie for
    the dictionary when compressing, and an array of strings when
    decompressing.
    
    The output of the compression is an array of fixed-length codewords.
    Writing them efficiently to a file is handled by a different
    component, io.c

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
        trie_put(dict, char_buf, 1);
        c++;
    } while (c != 0);

    // LZW Compression.
    char         substr[BUF_SIZE];
    unsigned int substr_len;
    unsigned int di = 0;        // Index in *dest

    substr_len = 0;
    for (unsigned int i = 0; i < len; i++) {
        c = src[i];
        substr[substr_len] = c;
        substr_len++;
        if (trie_get(dict, substr, substr_len) == 0) {
            dest[di] = trie_get(dict, substr, substr_len-1);
            di++;
            trie_put(dict, substr, substr_len);
            substr[0] = c;
            substr_len = 1;
        }
    }
    // Encode last codeword.
    dest[di] = trie_get(dict, substr, substr_len);
    di++;
    // Returns number of compressed codewords.
    return di;
}

unsigned int decompress(const codeword *src, unsigned int len, char *dest) {
    // Construct initial dictionary.
    unsigned int   dict_size = 256+1;
    unsigned int   dict_next = 256+1;
    // An array of unterminated strings.
    char         **dict      = malloc(sizeof(char*) * dict_size); 
    // The length of each string in dict, because they are not
    // null-terminated.
    unsigned int  *dict_lens = malloc(sizeof(unsigned int) * dict_size);
    // This time it's a short, because it iterates 1-256.
    short          c;           

    c = 1;
    do {
        dict[c] = malloc(sizeof(char));
        dict[c][0] = c-1;
        dict_lens[c] = 1;
        c++;
    } while (c <= 256);

    // LZW Decompression.
    codeword     cw;
    codeword     cw_prev;
    // For building the codeword translations.
    char         substr_ch[BUF_SIZE];
    unsigned int substr_ch_len = 0;
    // The last encoded string, used when resolving the exception
    // to the algorithm.
    char         encoded[BUF_SIZE];
    unsigned int encoded_len;
    unsigned int i  = 0; // Index in *src
    unsigned int di = 0; // Index in *dest

    cw_prev = src[i];
    i++;
    // Encode the first codeword.
    memcpy(dest+di, dict[cw_prev], dict_lens[cw_prev]);
    di += dict_lens[cw_prev];
    while (i < len) {
        cw = src[i];
        if (dict_lens[cw] > 0) {
            // The codeword is in the dictionary.
            // Encode it.
            memcpy(dest+di, dict[cw], dict_lens[cw]);
            di += dict_lens[cw];
            // Remember that it was encoded.
            // TODO: Remember only the index in the dictionary
            // instead of the whole string.
            memcpy(encoded, dict[cw], dict_lens[cw]);
            encoded_len = dict_lens[cw];
            // Build the next entry as entry + entry[0].
            c = dict[cw][0];
            memcpy(substr_ch, dict[cw_prev], dict_lens[cw_prev]);
            substr_ch[dict_lens[cw_prev]] = c;
            substr_ch_len = dict_lens[cw_prev] + 1;
        } else {
            // The codeword is NOT in the dictionary.
            // Encode the last encoded string, and repeat the
            // first character - used for resolving the exception.
            encoded[encoded_len] = encoded[0];
            encoded_len++;
            memcpy(dest+di, encoded, encoded_len);
            di += encoded_len;
        }

        if (dict_next == dict_size) {
            // There is no more space available in the dictionary,
            // so double it in size.
            dict_size *= 2;
            dict = realloc(dict, sizeof(char*) * dict_size);
            dict_lens = realloc(dict_lens, sizeof(unsigned int) * dict_size);
            // If a length of a string as specified in dict_lens is
            // 0, then the string is assumed to be missing.
            // So it's important to set the lengths of currently
            // missing entries to 0.
            memset(dict_lens+dict_next, 0, dict_size-dict_next);
        }
        // Add the current entry (substr_ch) to the dictionary.
        dict[dict_next] = malloc(sizeof(char) * substr_ch_len);
        memcpy(dict[dict_next], substr_ch, substr_ch_len);
        dict_lens[dict_next] = substr_ch_len;
        dict_next++;
        // Set the previous codeword.
        cw_prev = cw;
        // Move along in *src.
        i++;
    }

    // Return the number of decoded bytes.
    return di;
}
