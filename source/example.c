#include "stack.h"
#include <stdio.h>



int main (void)
{
    stack* stk = StackConstructor (sizeof (int));
    int output = 0;

    for (size_t i = 0; i < 16; ++i)
    {
        StackPush (stk, &i);
        StackTop (stk, &output);
        printf ("%d ", output);
    }

    printf ("\n");

    for (size_t i = 0; i < 16; ++i)
    {
        StackPop (stk);
        StackTop (stk, &output);
        printf ("%d ", output);
    }

    stk = StackDestructor (stk);

    return 0;
}
