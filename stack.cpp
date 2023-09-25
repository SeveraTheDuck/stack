#include "stack.h"

// realloc в мусоре - заполнить POISON
// для размера можно для ядовитости использовать INT_MAX, можно int для отрицательных
// -1U - беззнаковая -1, максимальное беззнаковое число
// -Е - раскрытие всех define. лучше при этом не использовать std
// если size < 0 и capacity < 0 не выводить массив, иначе - что-то можно вывести
//

ErrorType StackCtor (Stack* stk,  size_t stack_capacity,
                     const char*  init_name,
                     const size_t init_line,
                     const char*  init_file,
                     const char*  init_func)
{
    if (!stk)
    {
        fprintf (stderr, "STACK_NULL_POINTER\n");
        return STACK_NULL_POINTER;
    }

    stk->left_canary = CANARY_VALUE;
    stk->data = (Elem_t*) calloc (stack_capacity, sizeof(Elem_t));
    stk->data_size = 0;
    stk->data_capacity = stack_capacity;
    stk->init_name = init_name;
    stk->init_line = init_line;
    stk->init_file = init_file;
    stk->init_func = init_func;
    stk->right_canary = CANARY_VALUE;

    ErrorType stack_err = StackVerify(stk);
    if (stack_err)
    {
        STACKDUMP(stk);
        return stack_err;
    }

    return stack_err;
}

ErrorType StackDtor (Stack* stk)
{
    ErrorType stack_err = StackVerify(stk);
    if (stack_err)
    {
        STACKDUMP(stk);
    }

    free (stk->data);
    stk->data          = nullptr;
    stk->data_size     = 0;
    stk->data_capacity = 0;

    return stack_err;
}

ErrorType StackResize(Stack* stk, ResizeMode direction)
{
    ErrorType stack_err = StackVerify(stk);
    if (stack_err)
    {
        STACKDUMP(stk);
        return stack_err;
    }

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
    ErrorType stack_err = StackVerify(stk);
    if (stack_err)
    {
        STACKDUMP(stk);
        return stack_err;
    }

    if (stk->data_size == stk->data_capacity)
        StackResize(stk, EXPAND);

    stk->data[stk->data_size++] = value;

    return stack_err;
}

ErrorType StackPop (Stack* stk, Elem_t* return_value)
{
    ErrorType stack_err = StackVerify(stk);
    if (stack_err)
    {
        STACKDUMP(stk);
        return stack_err;
    }

    if (stk->data_size * 4 <= stk->data_capacity &&
                              stk->data_size >= INIT_CAPACITY)
        StackResize(stk, SHORTEN);

    *return_value = stk->data[--stk->data_size];
    stk->data[stk->data_size] = POISON;

    return stack_err;
}

ErrorType StackPrint (Stack* stk)
{
    ErrorType stack_err = StackVerify(stk);
    if (stack_err)
    {
        STACKDUMP(stk);
        return stack_err;
    }

    for (size_t i = 0; i < stk->data_size; ++i)
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
    if (stk->left_canary != CANARY_VALUE)
        stack_err |= (unsigned int) STACK_LEFT_CANARY_DAMAGED;
    if (stk->right_canary != CANARY_VALUE)
        stack_err |= (unsigned int) STACK_RIGHT_CANARY_DAMAGED;


    return (ErrorType) stack_err;
}

void StackDump (Stack* stk, const char* file_name,
                            const int line,
                            const char* func_name,
                            ErrorType stack_err)
{
    fprintf(stderr, "Stack [%p] \"%s\" from %s(%zd) %s()\n", stk, stk->init_name,
                                                           stk->init_file,
                                                           stk->init_line,
                                                           stk->init_func);
    fprintf(stderr, "called from %s(%d) %s()",  file_name, line, func_name);

    fprintf(stderr, "\n{\n");

    fprintf(stderr, "left_canary  = " CANARY_F, stk->left_canary);
    if (stk->left_canary == CANARY_VALUE)
        fprintf(stderr, " (STANDART VALUE)");
    fprintf(stderr, "\nright_canary = " CANARY_F, stk->right_canary);
    if (stk->right_canary == CANARY_VALUE)
        fprintf(stderr, " (STANDART VALUE)\n");

    fprintf(stderr, "size     = %zd\n", stk->data_size);
    fprintf(stderr, "capacity = %zd\n", stk->data_capacity);
    fprintf(stderr, "data[%p]\n", &stk->data);

    if (!stk->data)
    {
        StackErrorOutput((unsigned int) stack_err);
        return;
    }

    if (stk->data_capacity > 0)
    {
        fprintf(stderr, "\t{\n");
        for (size_t i = 0; i < stk->data_capacity; ++i)
        {
            if (stk->data[i] == POISON)
                fprintf(stderr, "\t[%zd] = " OUTPUT_F " (POISON)\n", i, stk->data[i]);
            else if ((long long)i < (long long)stk->data_size)                          // explicit cast is used for case data_size < 0
                fprintf(stderr, "\t*[%zd] = " OUTPUT_F "\n", i, stk->data[i]);
            else
                fprintf(stderr, "\t [%zd] = " OUTPUT_F "\n", i, stk->data[i]);
        }
        fprintf(stderr, "\t}\n");
    }

    else if (stk->data_size > 0)
    {
        fprintf(stderr, "\t{\n");
        for (size_t i = 0; i < stk->data_size; ++i)
        {
            fprintf(stderr, "*[%zd] = " OUTPUT_F "\n", i, stk->data[i]);
        }
        fprintf(stderr, "\t}\n");
    }

    else
    {
        fprintf(stderr, "Nothing to print!\n");
    }

    fprintf(stderr, "}\n");

    StackErrorOutput((unsigned int) stack_err);
}
//hex
void StackErrorOutput (unsigned int stack_err)
{
    fprintf(stderr, "\nSTACK ERRORS OCCURED:\n");
    if (stack_err & (unsigned int) STACK_NULL_POINTER)
        fprintf (stderr, "\tERROR 1: STACK_NULL_POINTER\n");

    if (stack_err & (unsigned int) STACK_DATA_NULL_POINTER)
        fprintf (stderr, "\tERROR 2: STACK_DATA_NULL_POINTER\n");

    if (stack_err & (unsigned int) STACK_SIZE_NOT_LEGIT_VALUE)
        fprintf (stderr, "\tERROR 3: STACK_SIZE_NOT_LEGIT_VALUE\n");

    if (stack_err & (unsigned int) STACK_CAPACITY_NOT_LEGIT_VALUE)
        fprintf (stderr, "\tERROR 4: STACK_CAPACITY_NOT_LEGIT_VALUE\n");

    if (stack_err & (unsigned int) STACK_SIZE_OUT_OF_RANGE)
        fprintf (stderr, "\tERROR 5: STACK_SIZE_OUT_OF_RANGE\n");

    if (stack_err & (unsigned int) STACK_LEFT_CANARY_DAMAGED)
        fprintf (stderr, "\tERROR 6: STACK_LEFT_CANARY_DAMAGED\n");

    if (stack_err & (unsigned int) STACK_RIGHT_CANARY_DAMAGED)
        fprintf (stderr, "\tERROR 7: STACK_RIGHT_CANARY_DAMAGED\n");

    fprintf(stderr, "\n");
}
