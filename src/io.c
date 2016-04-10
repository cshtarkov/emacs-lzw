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

    This module handles reading and writing the codeword array
    to files in an efficient manner. This could be handled by
    Emacs itself, but having it implemented in C:

    1) Allows for the program to be used standalone outside of Emacs.
    2) Allows for better control of the IO stream.

    A drawback is that it doesn't work with TRAMP.
    You can save the file to a temporary location on the
    local filesystem, and then copy it over using TRAMP.

*/

#include "io.h"
#include <stdlib.h>

int io_write_fd(FILE *fd, const codeword *code, unsigned int len) {
    // TODO: Rewrite this to find out the length of the longest
    // codeword in bits (the longest one will be the last one)
    // and shrink all codewords to be that size to save space.
    // Will need to transform the codeword[] array into a byte[]
    // array.
    if ((fwrite(code, sizeof(codeword), len, fd)) != len) {
        return -1;
    }
    return 0;
}

int io_write_fname(const char *name, const codeword *code, unsigned int len) {
    FILE *fd = fopen(name, "w");
    if (fd == NULL) {
        return -1;
    }
    if ((io_write_fd(fd, code, len)) != 0) {
        return -1;
    }
    fclose(fd);
    return 0;
}

codeword* io_read_fd(FILE *fd) {
    unsigned int  len;
    codeword     *code;
    if ((fseek(fd, 0, SEEK_END)) != 0) {
        return NULL;
    }
    len = ftell(fd);
    if ((fseek(fd, 0, SEEK_SET)) != 0) {
        return NULL;
    }

    code = malloc(sizeof(codeword) * len);
    if ((fread(code, sizeof(codeword), len, fd)) != len) {
        free(code);
        return NULL;
    }
    return code;
}

codeword* io_read_fname(const char *name) {
    FILE *fd = fopen(name, "r");
    if (fd == NULL) {
        return NULL;
    }
    codeword *code = io_read_fd(fd);
    if (code == NULL) {
        return NULL;
    }
    fclose(fd);
    return code;
}
