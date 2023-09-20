#ifndef __STACK_H__
#define __STACK_H__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "errors.h"

typedef int Elem_t;
const Elem_t POISON = -31415926;
#define OUTPUT_F "%d"

const size_t INIT_CAPACITY = 2;

const size_t RESIZE_MULTIPLIER = 2;

struct Stack
{
    Elem_t* data;
    size_t  data_size;
    size_t  data_capacity;
};

enum ResizeMode
{
    SHORTEN = -1,
    EXPAND  = 1
};

enum ErrorType
{
    NO_ERROR                       = 0,
    STACK_NULL_POINTER             = 1,
    STACK_DATA_NULL_POINTER        = 2,
    STACK_SIZE_NOT_LEGIT_VALUE     = 4,
    STACK_CAPACITY_NOT_LEGIT_VALUE = 8,
    STACK_SIZE_OUT_OF_RANGE        = 16
};

ErrorType StackCtor (Stack* stk, size_t stack_capacity);

ErrorType StackDtor (Stack* stk);

ErrorType StackResize(Stack* stk, ResizeMode direction);

ErrorType StackPush (Stack* stk, Elem_t value);

ErrorType StackPop (Stack* stk, Elem_t* return_value);

ErrorType StackPrint (Stack* stk);

ErrorType StackVerify (Stack* stk);

#endif
