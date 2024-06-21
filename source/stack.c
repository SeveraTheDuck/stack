#include "stack.h"



//-----------------------------------------------------------------------------
// Stack structure using dynamic array
//-----------------------------------------------------------------------------

struct stack
{
    d_array_t *d_array;
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Interface implementation
//-----------------------------------------------------------------------------

stack*
StackConstructor (const size_t elem_size)
{
    stack* const stk = malloc (sizeof (stack));
    if (stk == NULL) return NULL;

    stk->d_array = DynamicArrayConstructor (elem_size);
    if (stk->d_array == NULL)
        return StackDestructor (stk);

    return stk;
}


stack*
StackDestructor (stack* const stk)
{
    if (stk == NULL) return NULL;

    stk->d_array = DynamicArrayDestructor (stk->d_array);
    free (stk);

    return NULL;
}


stack_error_t
StackTop (stack* const stk,
          void* const buffer)
{
    if (stk    == NULL ||
        buffer == NULL)
        return STACK_ERROR;

    d_array_t* const d_array = stk->d_array;

    const void* const elem_ptr =
        DynamicArrayGetElemPtrByIndex (d_array, d_array->size - 1);
    if (elem_ptr == NULL) return STACK_ERROR;

    memcpy (buffer, elem_ptr, d_array->element_size);

    return STACK_SUCCESS;
}


stack_error_t
StackPush (stack* const stk,
           const void* const element)
{
    if (stk     == NULL ||
        element == NULL)
        return STACK_ERROR;

    d_array_t* const d_array = stk->d_array;

    if (DynamicArrayPush (d_array, element) == DYNAMIC_ARRAY_SUCCESS)
        return STACK_SUCCESS;

    return STACK_ERROR;
}


stack_error_t
StackPop (stack* const stk)
{
    if (stk == NULL) return STACK_ERROR;

    d_array_t* const d_array = stk->d_array;

    if (DynamicArrayPop (d_array) == DYNAMIC_ARRAY_SUCCESS)
        return STACK_SUCCESS;

    return STACK_ERROR;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
