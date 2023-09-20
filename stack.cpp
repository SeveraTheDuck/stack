#include "stack.h"

ErrorType StackCtor (Stack* stk, size_t stack_capacity)
{
    ErrorType stack_err = NO_ERROR;

    stk->data = (Elem_t*) calloc (stack_capacity, sizeof(Elem_t));
    stk->data_size     = 0;
    stk->data_capacity = stack_capacity;

    return stack_err;
}

ErrorType StackDtor (Stack* stk)
{
    ErrorType stack_err = NO_ERROR;

    free (stk->data);
    stk->data          = nullptr;
    stk->data_size     = 0;
    stk->data_capacity = 0;

    return stack_err;
}

ErrorType StackResize(Stack* stk, ResizeMode direction)
{
    ErrorType stack_err = NO_ERROR;

    switch (direction)
    {
        case EXPAND:
            stk->data_capacity *= RESIZE_MULTIPLIER;
            stk->data = (Elem_t*) realloc (stk->data,
                                           stk->data_capacity * sizeof(Elem_t));
            break;
        case SHORTEN:
            stk->data_capacity /= RESIZE_MULTIPLIER;
            stk->data = (Elem_t*) realloc (stk->data,
                                           stk->data_capacity * sizeof(Elem_t));
            break;
    }

    return stack_err;
}

ErrorType StackPush (Stack* stk, Elem_t value)
{
    ErrorType stack_err = NO_ERROR;

    if (stk->data_size == stk->data_capacity)
        StackResize(stk, EXPAND);

    stk->data[stk->data_size++] = value;

    return stack_err;
}

ErrorType StackPop (Stack* stk, Elem_t* return_value)
{
    ErrorType stack_err = NO_ERROR;

    if (stk->data_size * 4 <= stk->data_capacity && stk->data_size >= INIT_CAPACITY)
        StackResize(stk, SHORTEN);

    *return_value = stk->data[--stk->data_size];
    stk->data[stk->data_size] = POISON;

    return stack_err;
}

ErrorType StackPrint (Stack* stk)
{
    ErrorType stack_err = NO_ERROR;

    for (size_t i = 0; i < stk->data_capacity; ++i)
    {
        printf (OUTPUT_F "\n", stk->data[i]);
    }
    printf ("\n");

    return stack_err;
}

ErrorType StackVerify (Stack* stk)
{
    unsigned int stack_err = NO_ERROR;

    if (!stk)
        stack_err |= (unsigned int) STACK_NULL_POINTER;
    if (!stk->data)
        stack_err |= (unsigned int) STACK_DATA_NULL_POINTER;
    if (stk->data_size < 0)
        stack_err |= (unsigned int) STACK_SIZE_NOT_LEGIT_VALUE;
    if (stk->data_capacity < 0)
        stack_err |= (unsigned int) STACK_CAPACITY_NOT_LEGIT_VALUE;
    if (stk->data_capacity < stk->data_size)
        stack_err |= (unsigned int) STACK_SIZE_OUT_OF_RANGE;
    return (ErrorType) stack_err;
}
