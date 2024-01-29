#include "stack.h"

#define STACK_VERIFY(stk)           \
    if (StackVerify (stk))          \
    {                               \
        StackDump (stk);            \
                                    \
        if (stk == nullptr)         \
            return STACK_NULLPTR;   \
        return stk -> error_status; \
    }

static void
StackPrintErrors (const stack_error_t error_code);

static stack_error_t
StackRealloc (stack* const stk);

stack_error_t
StackConstructor (stack* const stk, STACK_CTOR_RECIVE_INFO,
                  const size_t stack_elem_size)
{
    if (stk == nullptr) 
        return STACK_NULLPTR;

    stk -> data_array          = nullptr;
    stk -> data_array_capacity = STACK_DATA_ARRAY_INITIAL_CAPACITY;
    stk -> data_array_size     = STACK_NULL_SIZE;
    stk -> elem_size           = stack_elem_size;
    stk -> error_status        = NO_ERROR;

    stk -> init_name = stack_init_name;
    stk -> init_file = stack_init_file;
    stk -> init_func = stack_init_func;
    stk -> init_line = stack_init_line;

    if (StackRealloc (stk) == STACK_DATA_NULLPTR)
        return stk -> error_status;
    
    return NO_ERROR;
}

stack_error_t
StackDestructor (stack* const stk)
{
    if (stk == nullptr)
        return STACK_NULLPTR;

    size_t n_bytes = stk -> data_array_capacity * 
                     stk -> elem_size;

    stk -> data_array_capacity = STACK_NULL_SIZE;
    stk -> data_array_size     = STACK_NULL_SIZE;
    stk -> elem_size           = STACK_NULL_SIZE;
    stk -> error_status        = NO_ERROR;

    stk -> init_name = nullptr;
    stk -> init_file = nullptr;
    stk -> init_func = nullptr;
    stk -> init_line = STACK_NULL_SIZE;

    if (stk -> data_array != nullptr)
        memset (stk -> data_array, STACK_POISON_BYTE, n_bytes);

    free (stk -> data_array);

    return NO_ERROR;
}

stack_error_t
StackVerify (stack* const stk)
{
    if (stk == nullptr)
        return STACK_NULLPTR;

    if (stk -> data_array == nullptr)
        stk -> error_status |= STACK_DATA_NULLPTR;

    if (stk -> data_array_size > STACK_MAX_SIZE)
        stk -> error_status |= STACK_INVALID_SIZE;

    if (stk -> data_array_capacity > STACK_MAX_SIZE)
        stk -> error_status |= STACK_INVALID_CAPACITY;
    
    if (stk -> data_array_size > stk -> data_array_capacity)
        stk -> error_status |= STACK_SIZE_OUT_OF_RANGE;

    return stk -> error_status;
}

void
StackDump (const stack* const stk)
{
    if (stk == nullptr)
    {
        printf ("STACK_NULLPTR ERROR\n");
        return;
    }

    printf ("Stack named %s [%p]\n"
            "from file %s line %zd function %s\n",
            stk -> init_name, stk, stk -> init_file,
            stk -> init_line, stk -> init_func);

    if (stk -> data_array)
    {
        putchar ('{');
        putchar ('\n');

        size_t n_bytes = stk -> data_array_capacity * stk -> elem_size;

        for (size_t elem_index = 0;
                    elem_index < n_bytes;
                    elem_index += stk -> elem_size)
        {
            printf ("\t%zd: ", elem_index / stk -> elem_size);
            for (size_t byte = 0; byte < stk -> elem_size; byte++)
            {
                printf ("%0X ", *((char*) stk -> data_array + 
                                          elem_index + byte));
            }
            putchar ('\n');
        }

        putchar ('}');
        putchar ('\n');
    }

    StackPrintErrors (stk -> error_status);
}

stack_error_t
StackPush (stack*      const stk, 
           const void* const push_value)
{
    STACK_VERIFY (stk);

    /* realloc */
    if (stk -> data_array_size == stk -> data_array_capacity)
    {
        stk -> data_array_capacity *= STACK_DATA_ARRAY_EXPANSION_MULTIPLIER;
        if (StackRealloc (stk) == STACK_DATA_NULLPTR)
            return stk -> error_status;
    }

    /* push itself */
    void* const dest_ptr = 
        StackGetElemPtrByIndex (stk, stk -> data_array_size);
    stk -> data_array_size++;

    memcpy (dest_ptr, push_value, stk -> elem_size);

    return NO_ERROR;
}

stack_error_t
StackPop (stack* const stk,
          void*  const dest_ptr)
{
    STACK_VERIFY (stk);

    if (stk -> data_array_size == STACK_NULL_SIZE)
    {
        fprintf (stderr, "Nothing to pop in StackPop, return\n");
        return STACK_POP_ELEM_OUT_OF_RANGE;
    }

    /* pop itself and poisoning */
    void* const pop_ptr =
        StackGetElemPtrByIndex (stk, stk -> data_array_size - 1);

    memcpy (dest_ptr, pop_ptr, stk -> elem_size);
    memset (pop_ptr, STACK_POISON_BYTE, stk -> elem_size);

    stk -> data_array_size--;

    /* realloc */
    if (stk -> data_array_size < 
        stk -> data_array_capacity /  STACK_DATA_ARRAY_EXPANSION_MULTIPLIER)
    {
        stk -> data_array_capacity /= STACK_DATA_ARRAY_EXPANSION_MULTIPLIER;
        if (StackRealloc (stk) == STACK_DATA_NULLPTR)
            return stk -> error_status;
    }

    return NO_ERROR;
}

void* const
StackGetElemPtrByIndex (stack* const stk,
                        const size_t elem_index)
{
    if (StackVerify (stk))
    {
        StackDump (stk);
        return nullptr;
    }

    if (elem_index > stk -> data_array_capacity)
    {
        fprintf (stderr, "In function StackGetElemByIndex "
                         "elem_index out of range\n");
        return nullptr;
    }

    return (void*) ((char*) stk -> data_array + 
                            elem_index * stk -> elem_size);
}

static stack_error_t
StackRealloc (stack* const stk)
{
    assert (stk);

    const size_t n_bytes = stk -> data_array_capacity * 
                           stk -> elem_size;

    const size_t n_poison_bytes = n_bytes - stk -> data_array_size *
                                            stk -> elem_size;

    void* new_ptr = realloc (stk -> data_array, n_bytes);

    if (new_ptr == nullptr)
    {
        perror ("WARNING! StackRealloc returned nullptr\n");
        free (stk -> data_array);

        stk -> error_status |= STACK_DATA_NULLPTR;

        return STACK_DATA_NULLPTR;
    }

    stk -> data_array = new_ptr;

    /* poison new bytes */
    memset (StackGetElemPtrByIndex (stk, stk -> data_array_size),
            STACK_POISON_BYTE, n_poison_bytes);

    return NO_ERROR;
}

static void
StackPrintErrors (const stack_error_t error_code)
{
    if (error_code & STACK_DATA_NULLPTR)
        fprintf (stderr, "STACK_DATA_NULLPTR ERROR\n");

    if (error_code & STACK_INVALID_SIZE)
        fprintf (stderr, "STACK_INVALID_SIZE ERROR\n");

    if (error_code & STACK_INVALID_CAPACITY)
        fprintf (stderr, "STACK_INVALID_CAPACITY ERROR\n");

    if (error_code & STACK_SIZE_OUT_OF_RANGE)
        fprintf (stderr, "STACK_SIZE_OUT_OF_RANGE ERROR\n");
}