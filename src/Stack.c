#include <stdio.h>

#include "Stack.h"
#include "malloc.h"

Stack * createStack(int capacity)
{
    Stack * stack = (Stack *) malloc(sizeof(Stack));
    stack->m_top = -1;
    stack->m_capacity = capacity;
    stack->m_data = (long *) malloc(sizeof(long) * capacity);
    for (int i = 0; i < capacity; i++)
    {
        stack->m_data[i] = 0;
    }
    return stack;
}

int isEmpty(Stack * s)
{
    return s->m_top == -1;
}

int isFull(Stack * s)
{
    return s->m_top == s->m_capacity - 1;
}

void push(Stack * s, long item)
{
    if (isFull(s))
    {
        s->m_capacity++;
        s->m_data = (long *) realloc(s->m_data, 4 * s->m_capacity); // TODO: better realloc scheme needed
        if (!s->m_data)
        {
            printf("Stack.c: Reallocation failed.\n");
            exit(EXIT_FAILURE);
        }
    }
    
    s->m_top++;
    s->m_data[s->m_top] = item;
}

long pop(Stack * s)
{
    if (isEmpty(s)) return 0x00;

    long value = s->m_data[s->m_top];
    s->m_top--;
    return value;
}

long peek(Stack * s)
{
    if (isEmpty(s)) return 0x00;
    else return s->m_data[s->m_top];
}

void clear(Stack * s)
{
    s->m_top = -1;
}

void deallocate(Stack * s)
{
    free(s->m_data);
}

// #include <stdio.h>
// int main(void)
// {
//     Stack * s = createStack(10);

//     for (long i = 0; i < 100; i++)
//     {
//         printf("%ld ", i);
//         push(s, i);
//     }
//     printf("\n");
//     for (int i = 0; i < 100; i++)
//     {
//         printf("%d ", pop(s));
//     }

//     return 0;
// }