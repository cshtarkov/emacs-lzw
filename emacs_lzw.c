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

// This really should be part of the Emacs module interface,
// just like make_string.
emacs_value make_vector(emacs_env *env, const char *contents, ptrdiff_t length) {
    emacs_value i_make_vector = env->intern(env, "make-vector");
    emacs_value i_nil = env->intern(env, "nil");
    emacs_value args[] = { env->make_integer(env, length), i_nil };
    emacs_value vec = env->funcall(env, i_make_vector, 2, args);
    for(unsigned int i = 0; i < length; i++) {
        env->vec_set(env, vec, i, env->make_integer(env, contents[i]));
    }
    return vec;
}

emacs_value compress_string(emacs_env *env, ptrdiff_t nargs,
                                emacs_value args[], void *data) {
    ptrdiff_t     len;          // Length of the source string.
    codeword     *code;         // Compressed string (vector).
    char         *buf;          // Source string.
    unsigned int  dlen;         // Length of the compressed string (vector).
    char         *code_as_char; // Byte-wise representation of the codewords.

    // Read in source string.
    len = env->extract_integer(env, args[1]) + 1;
    code = malloc(sizeof(codeword) * len);
    buf = malloc(sizeof(char) * len);
    env->copy_string_contents(env, args[0], buf, &len);

    // Compress it.
    dlen = lzw_compress(buf, len, code);
    code_as_char = malloc(sizeof(codeword) * dlen);

    // Represent code[] in code_as_char[].
    // Each multibyte element of code is
    // represented by several consecutive bytes in
    // code_as_char.
    unsigned int i, j;
    codeword *cwp;
    for (i = 0, j = 0; i < dlen * sizeof(codeword); i += sizeof(codeword), j++) {
        cwp = (codeword*)(code_as_char+i);
        *cwp = code[j];
    }

    // Make a vector out of the compressed string
    // and cleanup.
    emacs_value compressed = make_vector(env, code_as_char, sizeof(codeword)*dlen);
    free(buf);
    free(code);
    free(code_as_char);
    // Purposefully ignore those.
    nargs = 0; data = 0;
    return compressed;
}

emacs_value decompress_string(emacs_env *env, ptrdiff_t nargs,
                                emacs_value args[], void *data) {
    ptrdiff_t    len;          // Length of the source vector.
    emacs_value  vec;          // Source vector.
    char        *code_as_char; // Contents of source vector.
    codeword    *code;         // Collapse multiple bytes into a single multibyte codeword.

    // Read in source vector.
    vec = args[0];
    len = env->vec_size(env, vec);
    code_as_char = malloc(sizeof(char) * len);
    code = malloc(sizeof(codeword) * (len/sizeof(codeword)));
    for (unsigned int i = 0; i < len; i++) {
        code_as_char[i] = env->extract_integer(env, env->vec_get(env, vec, i));
    }

    // Collapse bytes in code_as_char[] into
    // multibyte codewords in code[].
    unsigned int i, j;
    codeword *cwp;
    for (i = 0, j = 0; i < len; i += sizeof(codeword), j++) {
        cwp = (codeword*)(code_as_char+i);
        code[j] = *cwp;
    }
    len /= sizeof(codeword);
    
    unsigned int  str_len;      // Length of decompressed string.
    char         *str;          // Decompressed string.
    unsigned int  dlen;         // Bytes decompressed so far.

    // Make a generous assumption for the length of
    // the decompressed string. This will get doubled
    // when insufficient.
    str_len = sizeof(char) * (len*10);
    str = malloc(str_len);
    dlen = 0;

    // Try to decompress the string into str_len bytes.
    while ((dlen = lzw_decompress(code, len, str, str_len)) == str_len) {
        str_len *= 2;
        str = realloc(str, str_len);
    }

    // Convert the decompressed string into an Emacs string
    // and cleanup.
    emacs_value decompressed = env->make_string(env, str, dlen-1);
    free(str);
    free(code);
    free(code_as_char);
    // Purposefully ignore those.
    nargs = 0; data = 0;
    return decompressed;
}

int emacs_module_init(struct emacs_runtime *ert) {
    emacs_env *env = ert->get_environment(ert);

    // Construct functions and symbols.
    emacs_value f_compress_string = env->make_function(env, 2, 2,
                                                       compress_string,
                                                       NULL, NULL);
    emacs_value i_compress_string = env->intern(env, "lzw--compress-string");
    emacs_value f_decompress_string = env->make_function(env, 1, 1,
                                                         decompress_string,
                                                         NULL, NULL);
    emacs_value i_decompress_string = env->intern(env, "lzw--decompress-string");

    // Use `fset' to set the symbols to the functions.
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
