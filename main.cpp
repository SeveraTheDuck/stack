#include "headers/stack.h"

int main()
{
    Stack stk1 = {};
    STACK_CTOR (&stk1);

    StackPush  (&stk1, 10);
    StackPush  (&stk1, 20);
    StackPush  (&stk1, 30);
    StackPush  (&stk1, 40);
    StackPush  (&stk1, 50);
    StackPush  (&stk1, 60);

    *((char*)&stk1) = 10;

    int x = 0;
    StackPop   (&stk1, &x);
    printf(OUTPUT_F "\n\n", x);

    StackPrint (&stk1);

    StackDtor (&stk1);

///////////////////////////////////////////////////

    Stack stk2 = {};
    STACK_CTOR (&stk2);

    StackPush  (&stk2, 10);
    StackPush  (&stk2, 20);
    StackPush  (&stk2, 30);
    StackPush  (&stk2, 40);
    StackPush  (&stk2, 50);
    StackPush  (&stk2, 60);



    x = 0;
    StackPop   (&stk2, &x);
    printf(OUTPUT_F "\n\n", x);

    StackPrint (&stk2);

    StackDtor  (&stk2);

    return 0;
}
