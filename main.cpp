#include "headers/stack.h"
#include "headers/errors.h"

int main()
{
    Stack stk = {};
    STACK_CTOR (&stk);

    // I await your hacks :)

    STACK_DTOR (&stk);

    return 0;
}
