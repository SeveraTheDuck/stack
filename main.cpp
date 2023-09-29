#include "headers/stack.h"
#include "headers/errors.h"

int main()
{
    Stack stk = {};
    STACK_CTOR (&stk);

    StackPush  (&stk, 10);
    StackPush  (&stk, 20);
    StackPush  (&stk, 30);
    StackPush  (&stk, 40);
    StackPush  (&stk, 50);

    STACK_DUMP (&stk);

    STACK_DTOR (&stk);

    return 0;
}
