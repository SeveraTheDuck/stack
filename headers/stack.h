#ifndef __STACK_H__
#define __STACK_H__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef int Elem_t;
const Elem_t POISON = -31415926;
#define OUTPUT_F "%d"

#define STACK_CTOR(stk, stack_capacity) StackCtor((stk), stack_capacity, #stk,\
                                                  __LINE__, __FILE__, __func__)
#define STACKDUMP(stk) StackDump(stk, __FILE__, __LINE__, __func__, stack_err)

typedef unsigned long long Canary_t;
#define CANARY_F "%llu"
const Canary_t CANARY_VALUE = 31415926UL;

const size_t INIT_CAPACITY = 2;
const size_t RESIZE_MULTIPLIER = 2;

struct Stack
{
    Canary_t left_canary;
    Elem_t* data;
    size_t  data_size;
    size_t  data_capacity;
    size_t init_line;
    const char*  init_name;
    const char*  init_file;
    const char*  init_func;
    Canary_t right_canary;
};

enum ResizeMode
{
    SHORTEN = -1,
    EXPAND  = 1
};

enum ErrorType
{
    NO_ERROR                        = 0,
    STACK_NULL_POINTER              = 1,
    STACK_DATA_NULL_POINTER         = 2,
    STACK_SIZE_NOT_LEGIT_VALUE      = 4,
    STACK_CAPACITY_NOT_LEGIT_VALUE  = 8,
    STACK_SIZE_OUT_OF_RANGE         = 16,
    STACK_LEFT_CANARY_DAMAGED       = 32,
    STACK_RIGHT_CANARY_DAMAGED      = 64,
    STACK_DATA_LEFT_CANARY_DAMAGED  = 128,
    STACK_DATA_RIGHT_CANARY_DAMAGED = 256,
    STACK_HASH_DAMAGED              = 512
};

ErrorType StackCtor   (Stack* stk,  size_t stack_capacity,
                       const char*  init_name,
                       const size_t init_line,
                       const char*  file_name,
                       const char*  init_func);

ErrorType StackDtor   (Stack* stk);

ErrorType StackResize (Stack* stk, ResizeMode direction);

ErrorType StackPush   (Stack* stk, Elem_t value);

ErrorType StackPop    (Stack* stk, Elem_t* return_value);

ErrorType StackPrint  (Stack* stk);

ErrorType StackVerify (Stack* stk);

void StackDump (Stack* stk, const char* file_name,
                            const int line,
                            const char* func_name,
                            ErrorType stack_err);

void StackErrorOutput (unsigned int stack_err);

#endif
