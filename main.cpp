#include "stack.h"

int main()
{
    Stack stk = {.data = nullptr, .data_size = 0, .data_capacity = INIT_CAPACITY};
    StackCtor (&stk, INIT_CAPACITY);

    StackPush  (&stk, 10);
    StackPush  (&stk, 20);
    StackPush  (&stk, 30);
    StackPush  (&stk, 40);
    StackPush  (&stk, 50);
    StackPrint (&stk);

    Elem_t x;
    StackPop (&stk, &x);

    StackPrint (&stk);

    StackDtor (&stk);
    //printf("\n" OUTPUT_F "\n", x);

    return 0;
}
