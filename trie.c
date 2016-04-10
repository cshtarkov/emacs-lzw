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

   This is an implementation of the trie data structure
   that is used to represent the dictionary of the LZW algorithm.

   It's not a general purpose implementation, so some design choices
   have been made:
   1) It uses a linked list to contain all children of a given node.
   2) Nodes are always valid words (substrings), so there's no intermediary nodes.

*/

#include "trie.h"
#include <stdlib.h>
#include <string.h>

// Implementations of the structs required in the header.

struct trie_child {
    struct trie_child *next;
    struct trie_node *node;
    char c;
};

struct trie_node {
    codeword cw;
    char *str;
    struct trie_child *first_child;
};

struct trie {
    struct trie_node *root;
    codeword next_cw;
};

// Prototypes of internal functions.

TrieNode* trie_node_create    (const char *str, unsigned int len, codeword cw);
void      trie_node_add_child (TrieNode *node, char c, TrieNode *child);
TrieNode* lookup_child        (TrieNode *node, char c);
void      trie_node_destroy(TrieNode *node);
void      trie_child_destroy(TrieChild *child);

// API implementation.

Trie* trie_init() {
    Trie *t = malloc(sizeof(Trie));
    t->root = trie_node_create("", 0, 0);
    // The codeword 0 is reserved for "entry not found".
    t->next_cw = 1;
    return t;
}

void trie_put(Trie *t, const char *w, unsigned int wlen) {
    TrieNode *cursor = t->root;
    TrieNode *next;
    unsigned int i = 0;
    while (i < wlen && (next = lookup_child(cursor, w[i])) != NULL) {
        cursor = next;
        i++;
    }
    // Handle a special case: putting ""
    if (wlen == 0) {
        trie_node_add_child(cursor, '\0', trie_node_create("\0", 0, t->next_cw));
        t->next_cw++;
        return;
    }
    if (i == wlen) {
        // This can occur iff W is already in T.
        return;
    }
    // Because this trie is used only with LZW and every
    // node is a valid word, the cursor is now the parent of the new
    // node.
    // Also, i should now equal wlen-1.
    // Now, add a child to the parent (cursor), with edge w[i],
    // that is a node with the new word and next available codeword.
    trie_node_add_child(cursor, w[i], trie_node_create(w, wlen, t->next_cw));
    // Update the next code word.
    t->next_cw++;
}

codeword trie_get(Trie *t, const char *w, unsigned int wlen) {
    TrieNode *cursor = t->root;
    TrieNode *next;
    unsigned int i = 0;
    while (i < wlen && (next = lookup_child(cursor, w[i])) != NULL) {
        cursor = next;
        i++;
    }
    if (i == wlen) {
        return cursor->cw;
    }
    return 0;
}

void trie_destroy(Trie *t) {
    trie_node_destroy(t->root);
    free(t);
}

// Internal functions implementation.

TrieNode* trie_node_create(const char *str, unsigned int len, codeword cw) {
    TrieNode *tn = malloc(sizeof(TrieNode));
    tn->str = malloc(sizeof(char) * len);
    memcpy(tn->str, str, len);
    tn->cw = cw;
    tn->first_child = NULL;
    return tn;
}

TrieNode* lookup_child(TrieNode *node, char c) {
    TrieChild *cursor;
    for (cursor = node->first_child; cursor != NULL; cursor = cursor->next) {
        if(cursor->c == c) {
            return cursor->node;
        }
    }
    return NULL;
}

void trie_node_add_child(TrieNode *node, char c, TrieNode *child) {
    TrieChild *new = malloc(sizeof(TrieChild));
    new->next = node->first_child;
    new->node = child;
    new->c = c;
    node->first_child = new;
    TrieChild *cursor;
    for (cursor = node->first_child; cursor != NULL; cursor = cursor->next) {
    }
}

void trie_node_destroy(TrieNode *node) {
    free(node->str);
    TrieChild *cursor = node->first_child;
    TrieChild *next;
    while (cursor != NULL) {
        next = cursor->next;
        trie_child_destroy(cursor);
        cursor = next;
    }
    free(node);
}

void trie_child_destroy(TrieChild *child) {
    trie_node_destroy(child->node);
    free(child);
}
