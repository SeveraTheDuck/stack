#include "stack.h"
#include "config.h"
#include "errors.h"

Error_t StackCtor (Stack* stk,
                   const char*  init_name,
                   const size_t init_line,
                   const char*  init_file,
                   const char*  init_func,
                   size_t stack_capacity)
{
    stk->stack_err = {};

    if (!stk)
    {
        fprintf (stderr, "STACK_NULL_POINTER\n");
        stk->stack_err.STACK_ERROR_OCCURED = 1;
        stk->stack_err.STACK_NULL_POINTER  = 1;
        return stk->stack_err.STACK_ERROR_OCCURED;
    }

    stk->data_size = 0;
    stk->data_capacity = stack_capacity;
    stk->init_name = init_name;
    stk->init_line = init_line;
    stk->init_file = init_file;
    stk->init_func = init_func;

    StackDataAlloc (stk, nullptr);

    STACKVERIFY(stk);

    return stk->stack_err.STACK_ERROR_OCCURED;
}

Error_t StackDtor (Stack* stk)
{
    STACKVERIFY(stk);

    free ((char*) stk->data - sizeof(Canary_t));
    stk->data          = nullptr;

    stk->data_size     = 0;
    stk->data_capacity = 0;
    stk->init_line     = 0;
    stk->init_name     = nullptr;
    stk->init_file     = nullptr;
    stk->init_func     = nullptr;
    stk->stack_err     = {};

    stk->left_canary   = 0;
    stk->right_canary  = 0;

    stk                = nullptr;

    return 0;
}

Error_t StackDataAlloc (Stack* stk, Elem_t* allocated_memory)
{
    if (!stk)
    {
        fprintf (stderr, "STACK_NULL_POINTER\n");
        stk->stack_err.STACK_ERROR_OCCURED = 1;
        stk->stack_err.STACK_NULL_POINTER  = 1;
        return stk->stack_err.STACK_NULL_POINTER;
    }

    if ((stk->data_capacity * sizeof(Elem_t)) % sizeof(Canary_t))
    {
        stk->data_capacity += (sizeof(Canary_t) - (stk->data_capacity *
                               sizeof(Elem_t))  % sizeof(Canary_t)) /
                               sizeof (Elem_t);
    }

    stk->data = (Elem_t*) realloc (allocated_memory,
                                   2 * sizeof (Canary_t) +
                                   stk->data_capacity * sizeof (Elem_t));

    if (!stk->data)
    {
        fprintf (stderr, "STACK_DATA_NULL_POINTER\n");
        stk->stack_err.STACK_ERROR_OCCURED = 1;
        stk->stack_err.STACK_DATA_NULL_POINTER = 1;
        return stk->stack_err.STACK_DATA_NULL_POINTER;
    }

    stk->data = (Elem_t*) ((Canary_t*)(void*) stk->data + 1);

    FillCanary(stk);

    for (size_t i = stk->data_size; i < stk->data_capacity; ++i)
    {
        *(stk->data + i) = POISON;
    }

    STACKVERIFY(stk);

    return stk->stack_err.STACK_ERROR_OCCURED;
}

Error_t FillCanary(Stack* stk)
{
    stk->left_canary  = CANARY_VALUE;
    stk->right_canary = CANARY_VALUE;

    *((Canary_t*)(void*)  stk->data - 1)                  = CANARY_VALUE;
    *( Canary_t*)(void*) (stk->data + stk->data_capacity) = CANARY_VALUE;

    STACKVERIFY(stk);

    return stk->stack_err.STACK_ERROR_OCCURED;
}

Error_t StackResize(Stack* stk, ResizeMode direction)
{
    STACKVERIFY(stk);

    switch (direction)
    {
        case EXPAND:
            stk->data_capacity *= RESIZE_MULTIPLIER;
            break;

        case SHORTEN:
            stk->data_capacity /= RESIZE_MULTIPLIER;
            break;
    }

    StackDataAlloc(stk, (Elem_t*) ((Canary_t*)(void*) stk->data - 1));

    return stk->stack_err.STACK_ERROR_OCCURED;
}

Error_t StackPush (Stack* stk, Elem_t value)
{
    STACKVERIFY(stk);

    if (stk->data_size == stk->data_capacity)
    {
        StackResize(stk, EXPAND);
    }

    stk->data[stk->data_size++] = value;

    return stk->stack_err.STACK_ERROR_OCCURED;
}

Error_t StackPop (Stack* stk, Elem_t* return_value)
{
    STACKVERIFY(stk);

    if (INIT_CAPACITY <= stk->data_size &&
                         stk->data_size * RESIZE_MULTIPLIER * RESIZE_MULTIPLIER <=
                         stk->data_capacity)
    {
        StackResize(stk, SHORTEN);
    }

    *return_value = stk->data[--stk->data_size];
    stk->data[stk->data_size] = POISON;

    return stk->stack_err.STACK_ERROR_OCCURED;
}

Error_t StackPrint (Stack* stk)
{
    STACKVERIFY(stk);

    for (size_t i = 0; i < stk->data_size; ++i)
    {
        printf (OUTPUT_F "\n", stk->data[i]);
    }
    printf ("\n");

    return stk->stack_err.STACK_ERROR_OCCURED;
}
