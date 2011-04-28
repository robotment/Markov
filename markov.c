#include "markov.h"

char *NOWORD = "\n";

State *statable[HASHSIZE];

size_t hash(char *prefix[]);

/* Initialize statable
 */
static void initialize()
{
    int i = 0;

    for (; i < NPREF; i++) {
        statable[i] = NULL;
    }

    return;
}
/* lookup prefix in hash table.
 * return index if found,or NULL.
 * Create prefix if is_create exit.
 */
State *lookup(char *prefix[], int is_create)
{
    size_t h = 0;
    State *sp = NULL;
    int i = 0;

    h = hash(prefix);
    for (sp = statable[h]; sp != NULL; sp = sp->next) {
        for (i = 0; i < NPREF; i++)
            if (strcmp(prefix[i], sp->prefix[i]) != 0)
                break;
        if (i == NPREF) return sp;
    }

    if (is_create) {
        sp = (State *) malloc(sizeof(State));
        
        /* Don't suppose type of prefix is char*.
         * using sizeof(prefix) to get length of array is wrong. Why?
         */ 
        memcpy(sp->prefix, prefix, sizeof(prefix[0]) * NPREF);
        /*for (i = 0; i < NPREF; i++)
            sp->prefix[i] = prefix[i];*/

        sp->suffix = NULL;
        sp->next = statable[h];
        statable[h] = sp;
    }

    return sp;
}

#define MULTI 31
/* Hash functions.
 */
size_t hash(char *prefix[])
{
    size_t h = 0;
    char *p = NULL;
    int i = 0;

    for (i = 0; i < NPREF; i++)
        for (p = prefix[i]; *p != '\0'; p++)
            h = MULTI * h + *p;

    return h % HASHSIZE;
}
#undef MULTI

/* build the markov link.
 */
#define BUF 100
void build(char *prefix[], FILE *fp)
{
    char fmt[10];
    char buf[BUF];

    initialize();

    sprintf(fmt, "%%%ds", BUF - 1);
    while (fscanf(fp, fmt, buf) != EOF) {
        add(prefix, strdup(buf));
    }
    
    return;
}
#undef BUF

/* add suffix to prefix */
static void addsuffix(State *sp, char *suffix)
{
    Suffix * sufp = NULL;

    sufp = (Suffix *) malloc(sizeof(Suffix));
    sufp-> word = suffix;
    sufp->next = sp->suffix;
    sp->suffix = sufp;

    return;
}

/* add suffix to hash table
 */
void add(char *prefix[], char *suffix)
{
    State *sp = NULL;

    sp = lookup(prefix, 1);
    addsuffix(sp, suffix);

    memmove(prefix, prefix + 1, sizeof(prefix[0]) * (NPREF - 1));
    prefix[NPREF - 1] = suffix;

    return;
}


/* auto generate some text on screen.
 */
void generate(char *prefix[])
{
    State *sp = NULL;
    Suffix *suffix = NULL;
    int i = 0;
    int nmatch = 0;
    char *w;

    srand(getpid());
    for (i = 0; i < MAXGEN; i++) {
        sp = lookup(prefix, 0);
        
        if (sp == NULL) {
            fprintf(stderr, "sp is null\n");
            exit(EXIT_FAILURE);
        }
        for (suffix = sp->suffix; suffix != NULL; suffix = suffix->next)
            if (rand() % ++nmatch == 0)
                w = suffix->word;
        
        printf("%s ", w);
        if (strcmp(w, NOWORD) == 0)
            break;

        nmatch = 0;
        memmove(prefix, prefix + 1, sizeof(prefix[0]) * (NPREF -1));
        prefix[NPREF - 1] = w;
    }
    
    return;
}

/* free memory allocate by strdup!
 */
void str_free()
{
    int i = 0;
    State *sp = NULL;
    Suffix *suffix = NULL;

    for (i = 0; i < HASHSIZE; i++) {
        sp = statable[i];
        while (sp != NULL) {
            /* free suffix */
            while ((suffix = sp->suffix) != NULL) {
                if (strcmp(suffix->word, NOWORD) != 0)
                    free(suffix->word);
                sp->suffix = suffix->next;
                free(suffix);
            }

            sp = sp->next;
            free(statable[i]);
            statable[i] = sp;
        }
    }

    return;
}
