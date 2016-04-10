;;; emacs-lzw.el --- Provide LZW compression and decompression within Emacs

;; Copyright © 2016 Christian Shtarkov

;; Author: Christian Shtarkov <cshtarkov@gmail.com>
;; URL: https://github.com/cshtarkov/emacs-lzw

;; This file is part of emacs-lzw.

;; emacs-lzw is free software: you can redistribute it and/or modify it under
;; the terms of the GNU General Public License as published by the Free
;; Software Foundation, either version 3 of the License, or (at your
;;                                                              option) any later version.

;; emacs-lzw is distributed in the hope that it will be useful, but WITHOUT ANY
;; WARRANTY; without even the implied warranty of MERCHANTABILITY or
;; FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
;; for more details.

;; You should have received a copy of the GNU General Public License
;; along with emacs-lzw. If not, see http://www.gnu.org/licenses/.

;;; Commentary:

;; This is a set of interactive functions for text compression and
;; decompression using the popular dictionary-based Lempel-Ziv-Welch
;; algorithm.

;; All functions run inside of Emacs and do not depend on external
;; tools, such as zip or gzip. The algorithm itself is implemented in C
;; for performance reasons, and used in Emacs as an external module.

;; This requires at least Emacs 25, and compiling the module.
;; Compiling the module is done using clang by default, but the Makefile
;; can be easily modified if another compiler is desired (such as gcc).

;; For use of this package, examine all user-facing interactive functions
;; with the prefix `lzw-'.

;;; Code:

;; Variables

(defvar lzw-module-location (concat user-emacs-directory "modules/"))
(defvar lzw-module-name "emacs_lzw.so")

;; Internal Functions

(defun lzw--module-installed-p ()
  (file-exists-p (concat lzw-module-location lzw-module-name)))

(defun lzw--module-loaded-p ()
  (functionp 'lzw--compress-string))

(defmacro lzw--with-module (&rest forms)
  `(if (lzw--module-loaded-p)
       ((lambda () ,@forms))
     (error "%s is not loaded! Check `lzw-module-location'." lzw-module-name)))

;; User-Facing Functions

(defun lzw-compress-string (str)
  "Returns the compressed form of STR as another string."
  (lzw--with-module
   (let* ((vec (lzw--compress-string str (length str)))
          (unsigned (mapcar (lambda (x) (if (< x 0) (+ x 256) x)) vec)))
     (concat unsigned))))

(defun lzw-decompress-string (str)
  "Returns the decompressed form of STR as another string."
  (lzw--with-module
   (lzw--decompress-string (vconcat str))))

(defun lzw-compress-buffer ()
  "Compresses the contents of the current buffer."
  (interactive)
  (lzw--with-module
   (let ((contents (buffer-string))
     (erase-buffer)
     (insert (lzw-compress-string contents))))))

(defun lzw-decompress-buffer ()
  "Attempts to decompress the contents of the current buffer.
Will only work if they've been compressed with 
this LZW implementation."
  (interactive)
  (lzw--with-module
   (let ((contents (buffer-string)))
     (erase-buffer)
     (insert (lzw-decompress-string contents)))))
