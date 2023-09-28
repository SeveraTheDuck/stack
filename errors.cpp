#include "headers/stack.h"
#include "headers/errors.h"

static void CleanLogFile (const char* const log_file_name)
{
    assert(log_file_name);

    static bool is_cleaned = false;
    if (is_cleaned == true)
    {
        return;
    }
    FILE*   fp = fopen (log_file_name, "wb");
    fclose (fp);
    is_cleaned = true;
}

__attribute__((format(printf, 2, 3)))
static void PrintLog (const char* const log_file_name,
                      const char* const fmt, ...)
{
    assert(log_file_name);
    assert(fmt);

    CleanLogFile     (log_file_name);
    FILE* fp = fopen (log_file_name, "a");

    va_list args = {};

    va_start (args, fmt);

    vfprintf (stderr, fmt, args);
    vfprintf (fp, fmt, args);

    va_end   (args);
    fclose   (fp);
    fp = nullptr;
}

Error_t StackDataPtrError (Stack* stk)
{
    assert(stk);

    if (!stk->data)
    {
        PrintLog ("log.txt", "STACK_DATA_NULL_POINTER\n");
        stk->stack_err.STACK_ERROR_OCCURED = 1;
        stk->stack_err.STACK_DATA_NULL_POINTER = 1;
        return stk->stack_err.STACK_ERROR_OCCURED;
    }

    return 0;
}

Error_t StackIsAnyError (ErrorType* stack_err)
{
    assert(stack_err);

    if (stack_err->STACK_DATA_NULL_POINTER         ||
        stack_err->STACK_SIZE_NOT_LEGIT_VALUE      ||
        stack_err->STACK_CAPACITY_NOT_LEGIT_VALUE  ||
        stack_err->STACK_SIZE_OUT_OF_RANGE
#ifdef CANARY_PROTECTION
                                                   ||
        stack_err->STACK_LEFT_CANARY_DAMAGED       ||
        stack_err->STACK_RIGHT_CANARY_DAMAGED      ||
        stack_err->STACK_DATA_LEFT_CANARY_DAMAGED  ||
        stack_err->STACK_DATA_RIGHT_CANARY_DAMAGED
#endif

#ifdef HASH_PROTECTION
                                                   ||
        stack_err->STACK_HASH_DAMAGED
#endif
                                                    )
    {
        stack_err->STACK_ERROR_OCCURED = 1;
    }

    return stack_err->STACK_ERROR_OCCURED;
}

Error_t StackVerify (Stack* const stk)
{
    assert (stk);

    if (!stk->data)
    {
        stk->stack_err.STACK_DATA_NULL_POINTER         = 1;
    }

    if (stk->data_size < 0)
    {
        stk->stack_err.STACK_SIZE_NOT_LEGIT_VALUE      = 1;
    }

    if (stk->data_capacity < 0)
    {
        stk->stack_err.STACK_CAPACITY_NOT_LEGIT_VALUE  = 1;
    }

    if (stk->data_capacity < stk->data_size)
    {
        stk->stack_err.STACK_SIZE_OUT_OF_RANGE         = 1;
    }

#ifdef CANARY_PROTECTION
    if (stk->left_canary != CANARY_VALUE)
    {
        stk->stack_err.STACK_LEFT_CANARY_DAMAGED       = 1;
    }

    if (stk->right_canary != CANARY_VALUE)
    {
        stk->stack_err.STACK_RIGHT_CANARY_DAMAGED      = 1;
    }

    if (*((Canary_t*)(void*) stk->data - 1) != CANARY_VALUE)
    {
        stk->stack_err.STACK_DATA_LEFT_CANARY_DAMAGED  = 1;
    }

    if (*(Canary_t*)(void*) (stk->data + stk->data_capacity) != CANARY_VALUE)
    {
        stk->stack_err.STACK_DATA_RIGHT_CANARY_DAMAGED = 1;
    }
#endif

    return StackIsAnyError (&stk->stack_err);
}

void StackDump (Stack* const stk, const char* file_name,
                                  const int   line,
                                  const char* func_name)
{
    assert(stk);
    assert(file_name);
    assert(line);
    assert(func_name);

    StackDumpHeader (stk, file_name, line, func_name);

    PrintLog ("log.txt", "{\n");

    StackDumpInfo   (stk);

#ifdef CANARY_PROTECTION
    StackDumpCanary (stk);
#endif

#ifdef HASH_PROTECTION
    StackDumpHash   (stk);
#endif

    PrintLog ("log.txt", "}\n");

    StackErrorOutput (&stk->stack_err);
}

void StackDumpHeader (Stack* const stk, const char* file_name,
                                        const int   line,
                                        const char* func_name)
{
    assert(stk);
    assert(file_name);
    assert(line);
    assert(func_name);

    PrintLog ("log.txt", "Stack [%p] \"%s\" \n       from %s(%zd) %s()\n",
                                                           stk, stk->init_name,
                                                           stk->init_file,
                                                           stk->init_line,
                                                           stk->init_func);
    PrintLog ("log.txt", "called from %s(%d) %s()\n", file_name, line, func_name);
}

void StackDumpInfo (Stack* const stk)
{
    assert(stk);

    PrintLog ("log.txt", "\tsize     = %zd\n", stk->data_size);
    PrintLog ("log.txt", "\tcapacity = %zd\n", stk->data_capacity);
    PrintLog ("log.txt", "\n\tdata[%p]\n",     stk->data);

    if (!stk->data)
    {
        StackErrorOutput (&stk->stack_err);
        return;
    }

    StackDumpData (stk);

    PrintLog ("log.txt", "\t}\n");
}

void StackDumpData (Stack* const stk)
{
    assert(stk);

    PrintLog ("log.txt", "\t{\n");
    if (stk->data_capacity > 0)
    {
        for (size_t i = 0; i < stk->data_capacity; ++i)
        {
            if (stk->data[i] == POISON)
            {
                PrintLog ("log.txt", "\t\t[%zd] = " OUTPUT_F " (POISON)\n", i, stk->data[i]);
            }
            else if ((long long) i < (long long) stk->data_size) // explicit cast is used for case data_size < 0
            {
                PrintLog ("log.txt", "\t\t*[%zd] = " OUTPUT_F "\n", i, stk->data[i]);
            }
            else
            {
                PrintLog ("log.txt", "\t\t [%zd] = " OUTPUT_F "\n", i, stk->data[i]);
            }
        }
    }

    else if (stk->data_size > 0)
    {
        for (size_t i = 0; i < stk->data_size; ++i)
        {
            PrintLog ("log.txt", "\t\t*[%zd] = " OUTPUT_F "\n", i, stk->data[i]);
        }
    }

    else
    {
        PrintLog ("log.txt", "\t\tNothing to print!\n");
    }
}

#ifdef CANARY_PROTECTION
void StackDumpCanary (Stack* const stk)
{
    assert(stk);

    PrintLog ("log.txt", "left_stack_canary  = " CANARY_F, stk->left_canary);
    if (stk->left_canary == CANARY_VALUE)
    {
        PrintLog ("log.txt", " (STANDART VALUE)");
    }
    PrintLog ("log.txt", "\n");

    PrintLog ("log.txt", "right_stack_canary = " CANARY_F, stk->right_canary);
    if (stk->right_canary == CANARY_VALUE)
    {
        PrintLog ("log.txt", " (STANDART VALUE)");
    }
    PrintLog ("log.txt", "\n");

    if (!stk->data)
    {
        PrintLog ("log.txt", "Cannot reach data canaries, data is nullptr!\n");
        return;
    }

    PrintLog ("log.txt", "left_data_canary   = " CANARY_F,
                        *((Canary_t*)(void*)stk->data - 1));
    if (*((Canary_t*)(void*) stk->data - 1) == CANARY_VALUE)
    {
        PrintLog ("log.txt", " (STANDART VALUE)");
    }
    PrintLog ("log.txt", "\n");

    PrintLog ("log.txt", "right_data_canary  = " CANARY_F,
                        *(Canary_t*)(void*) (stk->data + stk->data_capacity));
    if (*(Canary_t*)(void*) (stk->data + stk->data_capacity) == CANARY_VALUE)
    {
        PrintLog ("log.txt", " (STANDART VALUE)");
    }
    PrintLog ("log.txt", "\n");
}
#endif

#ifdef HASH_PROTECTION
void StackDumpHash (Stack* const stk)
{
    PrintLog ("log.txt", "hash_value = %llu\n", stk->hash_value);
}
#endif

//hex
void StackErrorOutput (ErrorType* const stack_err)
{
    assert(stack_err);

    PrintLog ("log.txt", "\nSTACK ERRORS OCCURED:\n");

    if (stack_err->STACK_NULL_POINTER)
        PrintLog ("log.txt", "\tERROR 1: STACK_NULL_POINTER\n");

    if (stack_err->STACK_DATA_NULL_POINTER)
        PrintLog ("log.txt", "\tERROR 2: STACK_DATA_NULL_POINTER\n");

    if (stack_err->STACK_SIZE_NOT_LEGIT_VALUE)
        PrintLog ("log.txt", "\tERROR 3: STACK_SIZE_NOT_LEGIT_VALUE\n");

    if (stack_err->STACK_CAPACITY_NOT_LEGIT_VALUE)
        PrintLog ("log.txt", "\tERROR 4: STACK_CAPACITY_NOT_LEGIT_VALUE\n");

    if (stack_err->STACK_SIZE_OUT_OF_RANGE)
        PrintLog ("log.txt", "\tERROR 5: STACK_SIZE_OUT_OF_RANGE\n");

#ifdef CANARY_PROTECTION
    if (stack_err->STACK_LEFT_CANARY_DAMAGED)
        PrintLog ("log.txt", "\tERROR 6: STACK_LEFT_CANARY_DAMAGED\n");

    if (stack_err->STACK_RIGHT_CANARY_DAMAGED)
        PrintLog ("log.txt", "\tERROR 7: STACK_RIGHT_CANARY_DAMAGED\n");

    if (stack_err->STACK_DATA_LEFT_CANARY_DAMAGED)
        PrintLog ("log.txt", "\tERROR 8: STACK_DATA_LEFT_CANARY_DAMAGED\n");

    if (stack_err->STACK_DATA_RIGHT_CANARY_DAMAGED)
        PrintLog ("log.txt", "\tERROR 9: STACK_DATA_RIGHT_CANARY_DAMAGED\n");
#endif

#ifdef HASH_PROTECTION
    if (stack_err->STACK_HASH_DAMAGED)
        PrintLog ("log.txt", "\tERROR 10: STACK_HASH_DAMAGED\n");
#endif

    PrintLog ("log.txt", "\n");
}
