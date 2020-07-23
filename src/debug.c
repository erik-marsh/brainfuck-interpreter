#include <stdio.h>

#include "debug.h"

// make debug cmds do nothing if not specified
// #ifndef DEBUG
// void printDebug(unsigned char * mem, int len) { }
// void printDebug_l(long * mem, int len) { }
// void stepDebug() { }
// #else
void printDebug(unsigned char * mem, int len)
{
    for (int i = 0; i < len; i++)
    {
        printf("%d ", mem[i]);
    }
    printf("\n");
}

void printDebug_l(long * mem, int len)
{
    for (int i = 0; i < len; i++)
    {
        printf("%d ", (int) mem[i]);
    }
    printf("\n");
}

void stepDebug(void)
{
    getc(stdin);
}
//#endif