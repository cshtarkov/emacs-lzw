# emacs-lzw

This is an implementation of the popular Lempel-Ziv-Welch compression
algorithm that can run within Emacs.

![demo](http://i.imgur.com/pDREZuJ.gif)

It consists of C source code that gets compiled into a shared library
and can be loaded dynamically by Emacs (>25), and some Lisp code that
interfaces with the library to provide user-facing commands.

This was developed mainly for the learning experience of 1)
implementing LZW and 2) writing Emacs extensions in C. It may or may
not work for serious use.

## Installation

    git clone https://github.com/cshtarkov/emacs-lzw
    cd emacs-lzw
    make && make install
    
This will install the shared library to `~/.emacs.d/modules`. 
The default target in the Makefile also runs the tests associated with
the Trie and LZW implementations, to make sure the algorithm itself
works.

## Why not use an external tool?

It's true that this approach has some drawbacks:

- A critical bug in the C code can shutdown the whole Emacs.
- If the compression takes a lot of time, Emacs will lock up until it's done (this can be worked around by using [emacs-async](https://github.com/jwiegley/emacs-async)).

It's main advantage is that no external programs are necessary.
This can get useful in the context of running an [Emacs Lisp Machine](http://shtarkov.net/public/posts/emacs-lisp-machine.html) or even [running Emacs as PID 1](http://www.informatimago.com/linux/emacs-on-user-mode-linux.html?repost).

The implementation is also reasonably fast, as it is written in C and not Lisp.
