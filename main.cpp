#include "stack.h"

int main()
{
    Stack stk1 = {};
    STACK_CTOR (&stk1);

    StackPush  (&stk1, 10);
    StackPush  (&stk1, 20);
    StackPush  (&stk1, 30);
    StackPush  (&stk1, 40);
    StackPush  (&stk1, 50);
    StackPrint (&stk1);

    Elem_t x = 0;
    StackPop   (&stk1, &x);

    StackPrint (&stk1);

    StackDtor  (&stk1);

/////////////////////////////////////////////

    Stack stk2 = {};
    STACK_CTOR (&stk2);

    StackPush  (&stk2, 10);
    StackPush  (&stk2, 20);

    StackPrint (&stk2);

    Elem_t y = 0;
    stk2.data_capacity = 1;
    //stk2.left_canary = 0;

    StackPop   (&stk2, &y);
    StackPop   (&stk2, &y);
    StackPop   (&stk2, &y);

    StackDtor  (&stk2);

    return 0;
}
