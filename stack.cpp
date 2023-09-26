#include "stack.h"
#include "config.h"

// realloc в мусоре - заполнить POISON
// для размера можно для ядовитости использовать INT_MAX, можно int для отрицательных
// -1U - беззнаковая -1, максимальное беззнаковое число
// -Е - раскрытие всех define. лучше при этом не использовать std
//

ErrorType StackCtor (Stack* stk,
                     const char*  init_name,
                     const size_t init_line,
                     const char*  init_file,
                     const char*  init_func,
                     size_t stack_capacity)
{
    if (!stk)
    {
        fprintf (stderr, "STACK_NULL_POINTER\n");
        return STACK_NULL_POINTER;
    }

    stk->data_size = 0;
    stk->data_capacity = stack_capacity;
    stk->init_name = init_name;
    stk->init_line = init_line;
    stk->init_file = init_file;
    stk->init_func = init_func;

    stk->data = (Elem_t*) calloc (2 * sizeof (Canary_t) +
                                  stack_capacity * sizeof (Elem_t), 1);

    if (!stk->data)
    {
        fprintf (stderr, "STACK_DATA_NULL_POINTER\n");
        return STACK_DATA_NULL_POINTER;
    }

    stk->data = (Elem_t*) ((Canary_t*)(void*) stk->data + 1);

    stk->left_canary  = CANARY_VALUE;
    stk->right_canary = CANARY_VALUE;

    FillDataCanary(stk);

    STACKVERIFY(stk);

    return stack_err;
}

ErrorType StackDtor (Stack* stk)
{
    STACKVERIFY(stk);

    free ((char*) stk->data - sizeof(Canary_t));
    stk->data          = nullptr;
    stk->data_size     = 0;
    stk->data_capacity = 0;

    return stack_err;
}

ErrorType FillDataCanary(Stack* stk)
{
    *((Canary_t*)(void*)  stk->data - 1)                  = CANARY_VALUE;
    *( Canary_t*)(void*) (stk->data + stk->data_capacity) = CANARY_VALUE;

    STACKVERIFY(stk);

    return stack_err;
}

ErrorType StackResize(Stack* stk, ResizeMode direction)
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

    stk->data = (Elem_t*) ((Canary_t*) realloc ((char*) stk->data - sizeof (Canary_t),
                            stk->data_capacity * sizeof (Elem_t) +
                            2 * sizeof (Canary_t)) + 1);
    FillDataCanary(stk);

    return stack_err;
}

ErrorType StackPush (Stack* stk, Elem_t value)
{
    STACKVERIFY(stk);

    if (stk->data_size == stk->data_capacity)
        StackResize(stk, EXPAND);

    stk->data[stk->data_size++] = value;

    return stack_err;
}

ErrorType StackPop (Stack* stk, Elem_t* return_value)
{
    STACKVERIFY(stk);

    //fprintf(stderr, "%du I AM STACK_ERR", (unsigned int) stack_err);???
    // if (stk->data_size * 4 <= stk->data_capacity &&
    //                         stk->data_size >= INIT_CAPACITY)
    if (INIT_CAPACITY <= stk->data_size &&
                         stk->data_size * 4 <= stk->data_capacity)
        StackResize(stk, SHORTEN);

    *return_value = stk->data[--stk->data_size];
    stk->data[stk->data_size] = POISON;

    return stack_err;
}

ErrorType StackPrint (Stack* stk)
{
    STACKVERIFY(stk);

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
    if (*((Canary_t*)(void*) stk->data - 1) != CANARY_VALUE)
        stack_err |= (unsigned int) STACK_DATA_LEFT_CANARY_DAMAGED;
    if (*(Canary_t*)(void*) (stk->data + stk->data_capacity) != CANARY_VALUE)
        stack_err |= (unsigned int) STACK_DATA_RIGHT_CANARY_DAMAGED;

    return (ErrorType) stack_err;
}

void StackDump (Stack* stk, const char* file_name,
                            const int line,
                            const char* func_name,
                            ErrorType stack_err)
{
    fprintf(stderr, "Stack [%p] \"%s\" \n       from %s(%zd) %s()\n", stk, stk->init_name,
                                                           stk->init_file,
                                                           stk->init_line,
                                                           stk->init_func);
    fprintf(stderr, "called from %s(%d) %s()",  file_name, line, func_name);

    fprintf(stderr, "\n{\n");

    fprintf(stderr, "left_stack_canary  = " CANARY_F, stk->left_canary);
    if (stk->left_canary == CANARY_VALUE)
        fprintf(stderr, " (STANDART VALUE)");
    fprintf(stderr, "\n");
    fprintf(stderr, "right_stack_canary = " CANARY_F, stk->right_canary);
    if (stk->right_canary == CANARY_VALUE)
        fprintf(stderr, " (STANDART VALUE)");
    fprintf(stderr, "\n");

    fprintf(stderr, "left_data_canary   = " CANARY_F, *((Canary_t*)(void*)stk->data - 1));
    if (*((Canary_t*)(void*)stk->data - 1) == CANARY_VALUE)
        fprintf(stderr, " (STANDART VALUE)");
    fprintf(stderr, "\n");
    fprintf(stderr, "right_data_canary  = " CANARY_F, *(Canary_t*)(void*)(stk->data + stk->data_capacity));
    if (*(Canary_t*)(void*)(stk->data + stk->data_capacity) == CANARY_VALUE)
        fprintf(stderr, " (STANDART VALUE)");
    fprintf(stderr, "\n");

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

    if (stack_err & (unsigned int) STACK_DATA_LEFT_CANARY_DAMAGED)
        fprintf (stderr, "\tERROR 6: STACK_DATA_LEFT_CANARY_DAMAGED\n");

    if (stack_err & (unsigned int) STACK_DATA_RIGHT_CANARY_DAMAGED)
        fprintf (stderr, "\tERROR 7: STACK_DATA_RIGHT_CANARY_DAMAGED\n");

    fprintf(stderr, "\n");
}
