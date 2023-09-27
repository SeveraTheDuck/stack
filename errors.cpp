#include "stack.h"
#include "errors.h"

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

    fprintf(stderr, "left_data_canary   = " CANARY_F,
                        *((Canary_t*)(void*)stk->data - 1));
    if (*((Canary_t*)(void*)stk->data - 1) == CANARY_VALUE)
    {
        fprintf(stderr, " (STANDART VALUE)");
    }
    fprintf(stderr, "\n");

    fprintf(stderr, "right_data_canary  = " CANARY_F,
                        *(Canary_t*)(void*)(stk->data + stk->data_capacity));
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
