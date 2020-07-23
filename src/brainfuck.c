#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "Stack.h"
#include "debug.h"
#include "error.h"

#define DEBUG 0

const char * const PROGRAM_NAME    = "brainfuck.exe";
const char * const PROGRAM_VERSION = "v0.1.0";
const char * const USAGE_MESSAGE   = "Usage: brainfuck.exe path [--memory=N, --minify]";

typedef struct InterpVars_s
{
    const char * srcPath;
    unsigned int memory;
} InterpVars;

int parseArgs(const int argc, const char ** argv, InterpVars * vars);
char * parseFromFile(const char * srcPath);
void minify(const char * srcPath);

int main(const int argc, const char ** argv)
{
    InterpVars vars;
    
    if (parseArgs(argc, argv, &vars)) return 1;

    char * program = parseFromFile(vars.srcPath);
    if (!program) return 1;

    unsigned char * memory = (unsigned char *) calloc(vars.memory, sizeof(unsigned char));
    int dptr = 0;
    Stack * nestStack = createStack(30);

    if (!memory || !nestStack)
    {
        printf("Failed to create resources.");
        return 1;
    }
    
    // interpreter loop
    for (long ip = 0; program[ip] != '\0'; ip++)
    {
        switch (program[ip])
        {
        case '>':
            dptr++;
            if (dptr >= vars.memory)
            {
                errorMsg(PTR_OUT_OF_RANGE_RIGHT, vars.srcPath, ip);
                return 1;
            }
            break;
        case '<':
            dptr--;
            if (dptr < 0)
            {
                errorMsg(PTR_OUT_OF_RANGE_LEFT, vars.srcPath, ip);
                return 1;
            }
            break;
        case '+': // unsigned overflow behavior is predictable
            memory[dptr]++;
            break;
        case '-':
            memory[dptr]--;
            break;
        case '.':
            putc(memory[dptr], stdout);
            break;
        case ',':
            memory[dptr] = (unsigned char) getc(stdin);
            break;
        case '[':
            if (memory[dptr] == 0x00)
            {
                int nestLevel = 0;
                ip++; // consume the '[' character so we don't increment nestLevel immediately
                for (; program[ip] != '\0'; ip++)
                {
                    if (program[ip] == '[') nestLevel++;
                    else if (program[ip] == ']')
                    {
                        if (nestLevel == 0) break;
                        nestLevel--;
                    }
                }
            }
            else push(nestStack, ip);
            break;
        case ']':
            if (memory[dptr] != 0x00) ip = peek(nestStack);
            else pop(nestStack);
            break;
        default: break;
        }

        if (DEBUG)
        {
            printf("%c, %d\n", program[ip], dptr);
            printDebug(memory, 30);
            printDebug_l(nestStack->m_data, 10);
            stepDebug();
        }
    }

    deallocate(nestStack);
    free(nestStack);
    free(memory);
    free(program);
    return 0;
}

// returns 1 if the program needs to exit for any reason
int parseArgs(const int argc, const char ** argv, InterpVars * vars)
{
    if (argc < 2)
    {
        printf("%s %s\n", PROGRAM_NAME, PROGRAM_VERSION);
        printf("%s\n", USAGE_MESSAGE);
        return 1;
    }

    vars->srcPath = argv[1];

    if (argc == 3)
    {
        if (!strncmp("--memory=", argv[2], 9))
        {
            const char * numberStart = argv[2] + 9;
            int err = 0;

            if (*numberStart == '\0') err = 1;
            for (const char * ch = numberStart; *ch != '\0'; ch++)
                if (!(*ch >= '0' && *ch <= '9')) err = 1;

            if (err)
            {
                printf("Memory must be a positive number.\n");
                return 1;
            }

            vars->memory = atoi(numberStart);
            printf("Running with %d units of memory.\n", vars->memory);
        }
        else if (!strcmp("--minify", argv[2]))
        {
            minify(argv[1]);
            return 1;
        }
    }

    return 0;
}

// TODO: make sure there is no wasted memory after the realloc() call
char * parseFromFile(const char * srcPath)
{
    FILE * fp = fopen(srcPath, "rb");
    if (!fp)
    {
        printf("Failed to open file.\n");
        return NULL;
    }
    fseek(fp, 0L, SEEK_END);
    long fileSize = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    char * buffer = (char *) calloc(fileSize + 1, sizeof(char));
    if (!buffer)
    {
        printf("Unable to allocate memory for program.\n");
        return NULL;
    }

    int ch = 0;
    long pos = 0;
    long bracketStack = 0; // a "stack" to verify bracket balancing
    while ((ch = fgetc(fp)) != EOF)
    {
        switch (ch)
        {
        case '+': case '-':
        case '.': case ',':
        case '<': case '>':
            buffer[pos++] = (char) ch;
            break;
        case '[':
            bracketStack++;
            buffer[pos++] = (char) ch;
            break;
        case ']':
            if (bracketStack == 0)
            {
                errorMsg(UNBALANCED_BRACKETS, srcPath, pos);
                exit(EXIT_FAILURE);
            }
            else
            {
                bracketStack--;
            }
            buffer[pos++] = (char) ch;
            break;
        default: break;
        }
    }
    if (bracketStack != 0)
    {
        errorMsg(UNBALANCED_BRACKETS, srcPath, -1L);
        exit(EXIT_FAILURE);
    }
    buffer[pos] = '\0';

    buffer = (char *) realloc(buffer, pos + 1);
    if (!buffer)
    {
        printf("Unable to allocate memory for program.\n");
        return NULL; // i mean yeah i could just let this return one line later, but this is more clear to me
    }
    return buffer;
}

void minify(const char * srcPath)
{
    char * minifiedBuffer = parseFromFile(srcPath);
    if (!minifiedBuffer) exit(EXIT_FAILURE);

    FILE * fp = fopen("_minified.bf", "wb");
    if (!fp)
    {
        printf("Failed to write file.");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    fwrite(minifiedBuffer, sizeof(char), strlen(minifiedBuffer), fp);
    fclose(fp);
    return;
}