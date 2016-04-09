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

   Header file for the IO component. See io.c for implementation
   details.

*/

#ifndef IO_H
#define IO_H

#include <stdio.h>

#include "trie.h" // For the codeword type.

int       io_write_fd    (FILE *fd, const codeword *code, unsigned int len);
int       io_write_fname (const char *name, const codeword *code, unsigned int len);
codeword* io_read_fd     (FILE *fd);
codeword* io_read_fname  (const char *name);

#endif
