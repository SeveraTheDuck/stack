#include "headers/stack.h"
#include "headers/errors.h"

#ifdef HASH_PROTECTION
    #include "headers/hash.h"
#endif

Error_t StackCtor (Stack* const stk,
                   const char*  const init_name,
                   const size_t       init_line,
                   const char*  const init_file,
                   const char*  const init_func,
                   size_t stack_capacity)
{
    assert (stk);

    stk->data_size     = 0;
    stk->data_capacity = stack_capacity;
    stk->init_name     = init_name;
    stk->init_line     = init_line;
    stk->init_file     = init_file;
    stk->init_func     = init_func;
    stk->stack_err     = {};

#ifdef HASH_PROTECTION
    stk->hash_value = 0;
#endif

    StackDataAlloc (stk, nullptr);

    STACKVERIFY (stk);

    return stk->stack_err.STACK_ERROR_OCCURED;
}

Error_t StackDtor (Stack* stk)
{
    STACKVERIFY (stk);

#ifdef CANARY_PROTECTION
    stk->data = (Elem_t*) ((Canary_t*)(void*) stk->data - 1);
#endif

    free (stk->data);
    stk->data          = nullptr;

    stk->data_size     = 0;
    stk->data_capacity = 0;
    stk->init_line     = 0;
    stk->init_name     = nullptr;
    stk->init_file     = nullptr;
    stk->init_func     = nullptr;
    stk->stack_err     = {};

#ifdef CANARY_PROTECTION
    stk->left_canary   = 0;
    stk->right_canary  = 0;
#endif

#ifdef HASH_PROTECTION
    stk->hash_value    = 0;
#endif

    stk                = nullptr;

    return STACK_DTOR_NO_ERROR;
}

Error_t StackDataAlloc (Stack* const stk, Elem_t* const allocated_memory)
{
    assert (stk);

#ifdef CANARY_PROTECTION
    if ((stk->data_capacity  *  sizeof (Elem_t))  % sizeof (Canary_t))
    {
        stk->data_capacity +=  (sizeof (Canary_t) - (stk->data_capacity *
                                sizeof (Elem_t))  % sizeof (Canary_t)) /
                                sizeof (Elem_t);
    }
#endif

    size_t new_size = stk->data_capacity * sizeof (Elem_t);

#ifdef CANARY_PROTECTION
    new_size += 2 * sizeof (Canary_t);
#endif

    stk->data = (Elem_t*) realloc (allocated_memory, new_size);

    if (StackDataPtrError (stk))
    {
        return stk->stack_err.STACK_ERROR_OCCURED;
    }

#ifdef CANARY_PROTECTION
    stk->data = (Elem_t*) ((Canary_t*)(void*) stk->data + 1);
    FillCanary (stk);
#endif

    for (size_t i = stk->data_size; i < stk->data_capacity; ++i)
    {
        *(stk->data + i) = POISON;
    }

#ifdef HASH_PROTECTION
    stk->hash_value = StackFindHash (stk);
#endif

    STACKVERIFY (stk);

    return stk->stack_err.STACK_ERROR_OCCURED;
}

#ifdef CANARY_PROTECTION
Error_t FillCanary (Stack* const stk)
{
    stk->left_canary  = CANARY_VALUE;
    stk->right_canary = CANARY_VALUE;

    *((Canary_t*)(void*)  stk->data - 1)                  = CANARY_VALUE;
    *( Canary_t*)(void*) (stk->data + stk->data_capacity) = CANARY_VALUE;

    STACKVERIFY (stk);

    return stk->stack_err.STACK_ERROR_OCCURED;
}
#endif

Error_t StackResize (Stack* const stk, const ResizeMode direction)
{
    STACKVERIFY (stk);

    switch (direction)
    {
        case EXPAND:
            stk->data_capacity *= RESIZE_MULTIPLIER;
            break;

        case SHORTEN:
            stk->data_capacity /= RESIZE_MULTIPLIER;
            break;
    }

#ifdef CANARY_PROTECTION
    stk->data = (Elem_t*) ((Canary_t*)(void*) stk->data - 1);
#endif

    StackDataAlloc (stk, (Elem_t*) stk->data);

    return stk->stack_err.STACK_ERROR_OCCURED;
}

Error_t StackPush (Stack* const stk, const Elem_t value)
{
    STACKVERIFY (stk);

    if (stk->data_size == stk->data_capacity)
    {
        StackResize(stk, EXPAND);
    }

#ifdef HASH_PROTECTION
    char* hash_ptr = (char*) (stk->data + stk->data_size);
    for (size_t i = 0; i < sizeof (Elem_t); ++i)
    {
        stk->hash_value -= (Hash_t) *(hash_ptr + i);
    }
#endif

    stk->data[stk->data_size++] = value;

#ifdef HASH_PROTECTION
    stk->hash_value++;

    for (size_t i = 0; i < sizeof (Elem_t); ++i)
    {
        stk->hash_value += (Hash_t) *(hash_ptr + i);
    }

    StackHashError (stk);
#endif

    STACKVERIFY (stk);
    return stk->stack_err.STACK_ERROR_OCCURED;
}

Error_t StackPop (Stack* const stk, Elem_t* const return_value)
{
    STACKVERIFY (stk);

    if (INIT_CAPACITY <= stk->data_size &&
                         stk->data_size * RESIZE_MULTIPLIER * RESIZE_MULTIPLIER <=
                         stk->data_capacity)
    {
        StackResize (stk, SHORTEN);
    }

#ifdef HASH_PROTECTION
    char* hash_ptr = (char*) (stk->data + stk->data_size - 1);
    for (size_t i = 0; i < sizeof (Elem_t); ++i)
    {
        stk->hash_value -= (Hash_t) *(hash_ptr + i);
    }
#endif

    *return_value = stk->data[--stk->data_size];
    stk->data[stk->data_size] = POISON;

#ifdef HASH_PROTECTION
    stk->hash_value--;

    for (size_t i = 0; i < sizeof (Elem_t); ++i)
    {
        stk->hash_value += (Hash_t) *(hash_ptr + i);
    }

    StackHashError (stk);
#endif

    STACKVERIFY (stk);
    return stk->stack_err.STACK_ERROR_OCCURED;
}

Error_t StackPrint (Stack* const stk)
{
    STACKVERIFY (stk);

    for (size_t i = 0; i < stk->data_size; ++i)
    {
        printf (OUTPUT_F "\n", stk->data[i]);
    }
    printf ("\n");

    return stk->stack_err.STACK_ERROR_OCCURED;
}
