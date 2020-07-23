#include <stdio.h>

#include "error.h"

void errorMsg(const enum ErrorCode code, const char * srcFile, const long charPos)
{
    printf("In file %s", srcFile);
    if (charPos != -1L)
    {
        printf(", instruction no. %ld", charPos);
    }
    printf(":\nError parsing file: ");

    switch (code)
    {
    case PTR_OUT_OF_RANGE_LEFT:
        printf("Data pointer out of range (becomes -1).\n");
        break;
    case PTR_OUT_OF_RANGE_RIGHT:
        printf("Data pointer out of range (exceeds maximum range of memory tape).");
        break;
    case UNBALANCED_BRACKETS:
        printf("Brackets are unbalanced.");
        break;
    default:
        printf("Unknown error.\n");
        printf("This is likely a bug with the interpreter.\n");
    }
}