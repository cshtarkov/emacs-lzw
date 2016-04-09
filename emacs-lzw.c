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

    This is the Emacs module that is compiled into a shared
    library and can be loaded dynamically with Lisp:

    (load-file "emacs-lzw.so")

    It uses the functionality in lzw.h and io.h and exports
    the user-facing Lisp functions.

*/

#include "emacs-module.h"
#include "lzw.h"
#include "io.h"
#include <stdlib.h>
#include <stdio.h>

// Need to have a symbol of this name in the symbol
// table, otherwise Emacs will refuse to load the module.
int plugin_is_GPL_compatible;

emacs_value compress_string(emacs_env *env, ptrdiff_t nargs,
                                emacs_value args[], void *data) {
    ptrdiff_t len = env->extract_integer(env, args[1]);
    len++;
    codeword *code = malloc(sizeof(codeword) * len);
    char *buf = malloc(sizeof(char) * len);
    env->copy_string_contents(env, args[0], buf, &len);
    unsigned int dlen = lzw_compress(buf, len, code);
    char *code_as_char = malloc(sizeof(codeword) * dlen);

    unsigned int i;
    unsigned int k;
    unsigned int j;
    codeword mask;
    codeword cw;
    for (i = 0, k = 0; i < dlen; i++) {
        mask = 255;
        printf("Code is %d\n", code[i]);
        for (j = 0; j < sizeof(codeword); j++, k++) {
            cw = code[i];
            cw = cw & mask;
            cw = cw >> (j*8);
            code_as_char[k] = cw;
            printf("Bits (%d): %d\n", j, code_as_char[k]);
            mask = mask << 8;
        }
    }
    emacs_value compressed = env->make_string(env, code_as_char, sizeof(codeword)*dlen);
    free(buf);
    free(code);
    nargs = 0; data = 0;
    
    return compressed;
}

emacs_value decompress_string(emacs_env *env, ptrdiff_t nargs,
                                emacs_value args[], void *data) {
    ptrdiff_t len = env->extract_integer(env, args[1]);
    len++;
    char *code_as_char = malloc(sizeof(char) * len);
    codeword *code = malloc(sizeof(codeword) * (len/sizeof(codeword)));

    env->copy_string_contents(env, args[0], code_as_char, &len);


    // This assumes a 4 byte codeword!
    unsigned int i;
    unsigned int j;
    for (i = 0, j = 0; i < len; i+=4,j++) {
        printf("Bits (0): %d", (codeword)code_as_char[i]);
        printf("Bits (1): %d", (codeword)code_as_char[i+1]<<8);
        printf("Bits (2): %d", (codeword)code_as_char[i+2]<<16);
        printf("Bits (3): %d", (codeword)code_as_char[i+3]<<24);
        code[j] = (codeword)code_as_char[i]         |
                  (codeword)code_as_char[i+1] << 8  |
                  (codeword)code_as_char[i+2] << 16 |
                  (codeword)code_as_char[i+3] << 24;
        printf("Code is: %d", code[j]);
        printf("\n\n");
    }
    
    unsigned int str_len = sizeof(char) * (len*10);
    char *str = malloc(sizeof(char) * str_len);
    unsigned int dlen = 0;
    while ((dlen = lzw_decompress(code, len, str, str_len)) == str_len) {
        return env->make_string(env, "double", 6);
        str_len *= 2;
        str = realloc(str, sizeof(char) * str_len);
    }
    for(i = 0; i < dlen; i++) {
        printf("%c", str[i]);
    }
    printf("\n");
    emacs_value decompressed = env->make_string(env, str, dlen-1);
    free(str);
    free(code);
    nargs = 0; data = 0;
    return decompressed;
}

int emacs_module_init(struct emacs_runtime *ert) {
    emacs_env *env = ert->get_environment(ert);

    emacs_value f_compress_string = env->make_function(env, 2, 2,
                                                       compress_string,
                                                       NULL, NULL);
    emacs_value i_compress_string = env->intern(env, "lzw--compress-string");
    emacs_value f_decompress_string = env->make_function(env, 2, 2,
                                                         decompress_string,
                                                         NULL, NULL);
    emacs_value i_decompress_string = env->intern(env, "lzw--decompress-string");

    emacs_value i_fset = env->intern(env, "fset");
    emacs_value args_compress_string[] = {
        i_compress_string,
        f_compress_string
    };
    emacs_value args_decompress_string[] = {
        i_decompress_string,
        f_decompress_string
    };
    env->funcall(env, i_fset, 2, args_compress_string);
    env->funcall(env, i_fset, 2, args_decompress_string);

    return 0;
}
