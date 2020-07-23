#pragma once

typedef struct Stack_s
{
    int m_top;
    int m_capacity;
    long * m_data;
} Stack;

Stack * createStack(int capacity);
int isEmpty(Stack * s);
int isFull(Stack * s);
void push(Stack * s, long item);
long pop(Stack * s);
long peek(Stack * s);
void clear(Stack * s);
void deallocate(Stack * s);