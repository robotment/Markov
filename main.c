#include "markov.h"

int main()
{
    extern char *NOWORD;

    char *prefix[NPREF];
    int i = 0;

    for (i = 0; i < NPREF; i++)
        prefix[i] = NOWORD;

    build(prefix, stdin);
    add(prefix, NOWORD);

    for (i = 0; i < NPREF; i++)
        prefix[i] = NOWORD;

    generate(prefix);

    return 0;
}
