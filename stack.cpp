#include "stack.h"
#include "config.h"

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
    fprintf(stderr, "%d\n", stk->stack_err.STACK_ERROR_OCCURED);

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

Error_t StackVerify (Stack* stk)
{
    if (!stk)
    {
        stk->stack_err.STACK_ERROR_OCCURED             = 1;
        stk->stack_err.STACK_NULL_POINTER              = 1;
    }

    if (!stk->data)
    {
        stk->stack_err.STACK_ERROR_OCCURED             = 1;
        stk->stack_err.STACK_DATA_NULL_POINTER         = 1;
    }

    if (stk->data_size < 0)
    {
        stk->stack_err.STACK_ERROR_OCCURED             = 1;
        stk->stack_err.STACK_SIZE_NOT_LEGIT_VALUE      = 1;
    }

    if (stk->data_capacity < 0)
    {
        stk->stack_err.STACK_ERROR_OCCURED             = 1;
        stk->stack_err.STACK_CAPACITY_NOT_LEGIT_VALUE  = 1;
    }

    if (stk->data_capacity < stk->data_size)
    {
        stk->stack_err.STACK_ERROR_OCCURED             = 1;
        stk->stack_err.STACK_SIZE_OUT_OF_RANGE         = 1;
    }

    if (stk->left_canary != CANARY_VALUE)
    {
        stk->stack_err.STACK_ERROR_OCCURED             = 1;
        stk->stack_err.STACK_LEFT_CANARY_DAMAGED       = 1;
    }

    if (stk->right_canary != CANARY_VALUE)
    {
        stk->stack_err.STACK_ERROR_OCCURED             = 1;
        stk->stack_err.STACK_RIGHT_CANARY_DAMAGED      = 1;
    }

    if (*((Canary_t*)(void*) stk->data - 1) != CANARY_VALUE)
    {
        stk->stack_err.STACK_ERROR_OCCURED             = 1;
        stk->stack_err.STACK_DATA_LEFT_CANARY_DAMAGED  = 1;
    }

    if (*(Canary_t*)(void*) (stk->data + stk->data_capacity) != CANARY_VALUE)
    {
        stk->stack_err.STACK_ERROR_OCCURED             = 1;
        stk->stack_err.STACK_DATA_RIGHT_CANARY_DAMAGED = 1;
    }

    return stk->stack_err.STACK_ERROR_OCCURED;
}

void StackDump (Stack* stk, const char* file_name,
                            const int line,
                            const char* func_name)
{
    StackDumpHeader (stk, file_name, line, func_name);

    fprintf(stderr, "{\n");

    StackDumpInfo (stk);

    StackDumpCanary (stk);

    fprintf(stderr, "}\n");

    StackErrorOutput(&stk->stack_err);
}

void StackDumpHeader (Stack* stk, const char* file_name,
                                  const int line,
                                  const char* func_name)
{
    fprintf(stderr, "Stack [%p] \"%s\" \n       from %s(%zd) %s()\n",
                                                           stk, stk->init_name,
                                                           stk->init_file,
                                                           stk->init_line,
                                                           stk->init_func);
    fprintf(stderr, "called from %s(%d) %s()\n",  file_name, line, func_name);
}

void StackDumpCanary (Stack* stk)
{
    fprintf(stderr, "left_stack_canary  = " CANARY_F, stk->left_canary);
    if (stk->left_canary == CANARY_VALUE)
    {
        fprintf(stderr, " (STANDART VALUE)");
    }
    fprintf(stderr, "\n");

    fprintf(stderr, "right_stack_canary = " CANARY_F, stk->right_canary);
    if (stk->right_canary == CANARY_VALUE)
    {
        fprintf(stderr, " (STANDART VALUE)");
    }
    fprintf(stderr, "\n");

    if (!stk->data)
    {
        fprintf(stderr, "Cannot reach data canaries, data is nullptr!\n");
        return;
    }

    fprintf(stderr, "left_data_canary   = " CANARY_F, *((Canary_t*)(void*)stk->data - 1));
    if (*((Canary_t*)(void*)stk->data - 1) == CANARY_VALUE)
    {
        fprintf(stderr, " (STANDART VALUE)");
    }
    fprintf(stderr, "\n");

    fprintf(stderr, "right_data_canary  = " CANARY_F, *(Canary_t*)(void*)(stk->data + stk->data_capacity));
    if (*(Canary_t*)(void*)(stk->data + stk->data_capacity) == CANARY_VALUE)
    {
        fprintf(stderr, " (STANDART VALUE)");
    }
    fprintf(stderr, "\n");
}

void StackDumpInfo (Stack* stk)
{
    fprintf(stderr, "\tsize     = %zd\n", stk->data_size);
    fprintf(stderr, "\tcapacity = %zd\n", stk->data_capacity);
    fprintf(stderr, "\n\tdata[%p]\n",     stk->data);

    if (!stk->data)
    {
        StackErrorOutput(&stk->stack_err);
        return;
    }

    StackDumpData(stk);

    fprintf(stderr, "\t}\n");
}

void StackDumpData (Stack* stk)
{
    fprintf(stderr, "\t{\n");
    if (stk->data_capacity > 0)
    {
        for (size_t i = 0; i < stk->data_capacity; ++i)
        {
            if (stk->data[i] == POISON)
            {
                fprintf(stderr, "\t\t[%zd] = " OUTPUT_F " (POISON)\n", i, stk->data[i]);
            }
            else if ((long long)i < (long long)stk->data_size) // explicit cast is used for case data_size < 0
            {
                fprintf(stderr, "\t\t*[%zd] = " OUTPUT_F "\n", i, stk->data[i]);
            }
            else
            {
                fprintf(stderr, "\t\t [%zd] = " OUTPUT_F "\n", i, stk->data[i]);
            }
        }
    }

    else if (stk->data_size > 0)
    {
        for (size_t i = 0; i < stk->data_size; ++i)
        {
            fprintf(stderr, "\t\t*[%zd] = " OUTPUT_F "\n", i, stk->data[i]);
        }
    }

    else
    {
        fprintf(stderr, "\t\tNothing to print!\n");
    }
}
//hex
void StackErrorOutput (ErrorType* stack_err)
{
    fprintf(stderr, "\nSTACK ERRORS OCCURED:\n");

    if (stack_err->STACK_NULL_POINTER)
        fprintf (stderr, "\tERROR 1: STACK_NULL_POINTER\n");

    if (stack_err->STACK_DATA_NULL_POINTER)
        fprintf (stderr, "\tERROR 2: STACK_DATA_NULL_POINTER\n");

    if (stack_err->STACK_SIZE_NOT_LEGIT_VALUE)
        fprintf (stderr, "\tERROR 3: STACK_SIZE_NOT_LEGIT_VALUE\n");

    if (stack_err->STACK_CAPACITY_NOT_LEGIT_VALUE)
        fprintf (stderr, "\tERROR 4: STACK_CAPACITY_NOT_LEGIT_VALUE\n");

    if (stack_err->STACK_SIZE_OUT_OF_RANGE)
        fprintf (stderr, "\tERROR 5: STACK_SIZE_OUT_OF_RANGE\n");

    if (stack_err->STACK_LEFT_CANARY_DAMAGED)
        fprintf (stderr, "\tERROR 6: STACK_LEFT_CANARY_DAMAGED\n");

    if (stack_err->STACK_RIGHT_CANARY_DAMAGED)
        fprintf (stderr, "\tERROR 7: STACK_RIGHT_CANARY_DAMAGED\n");

    if (stack_err->STACK_DATA_LEFT_CANARY_DAMAGED)
        fprintf (stderr, "\tERROR 6: STACK_DATA_LEFT_CANARY_DAMAGED\n");

    if (stack_err->STACK_DATA_RIGHT_CANARY_DAMAGED)
        fprintf (stderr, "\tERROR 7: STACK_DATA_RIGHT_CANARY_DAMAGED\n");

    fprintf(stderr, "\n");
}
