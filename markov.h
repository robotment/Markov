#ifndef MARKOV_H
#define MARKOV_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
    NPREF = 2, /* prefix number */
    HASHSIZE = 4093, /* hash table size */
    MAXGEN = 10000 /*MAXIMUM number word to gernerated */
};

typedef struct _State State;
typedef struct _Suffix Suffix;
struct _State {
    char *prefix[NPREF];
    Suffix *suffix;
    State *next;
};

struct _Suffix {
    char *word;
    Suffix *next;
};

void build(char *prefix[], FILE *fp);
void add(char *prefix[], char *suffix);
State *lookup(char *prefix[], int is_create);
void generate(char *preifx[]);

#endif
