#ifndef __STACK_H__
#define __STACK_H__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "config.h"

#define STACK_CTOR(stk) StackCtor((stk), #stk,\
                                   __LINE__, __FILE__, __func__)

#define STACK_CTOR_CAP(stk, stack_capacity) StackCtor((stk), #stk,\
                                                       __LINE__, __FILE__, __func__,\
                                                       stack_capacity)

#define STACKDUMP(stk) StackDump(stk, __FILE__, __LINE__, __func__, stack_err)

#define STACKVERIFY(stk)\
    ErrorType stack_err = StackVerify(stk);\
    if (stack_err)\
    {\
        STACKDUMP(stk);\
        return stack_err;\
    }

typedef unsigned long long Canary_t;
#define CANARY_F "%llu"
const Canary_t CANARY_VALUE = 31415926UL;


struct Stack
{
    Canary_t left_canary;
    Elem_t*  data;
    size_t   data_size;
    size_t   data_capacity;
    size_t   init_line;
    const char* init_name;
    const char* init_file;
    const char* init_func;
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

ErrorType StackCtor      (Stack* stk,
                          const char*  init_name,
                          const size_t init_line,
                          const char*  file_name,
                          const char*  init_func,
                          size_t stack_capacity = INIT_CAPACITY);

ErrorType StackDtor      (Stack* stk);

ErrorType StackDataAlloc (Stack* stk, Elem_t* allocated_memory);

ErrorType FillCanary     (Stack* stk);

ErrorType StackResize    (Stack* stk, ResizeMode direction);

ErrorType StackPush      (Stack* stk, Elem_t value);

ErrorType StackPop       (Stack* stk, Elem_t* return_value);

ErrorType StackPrint     (Stack* stk);

ErrorType StackVerify    (Stack* stk);

void StackDump           (Stack* stk, const char* file_name,
                                      const int line,
                                      const char* func_name,
                                      ErrorType stack_err);

void StackDumpHeader     (Stack* stk, const char* file_name,
                                      const int line,
                                      const char* func_name);

void StackDumpCanary     (Stack* stk);

void StackDumpInfo       (Stack* stk, ErrorType stack_err);

void StackDumpData       (Stack* stk);

void StackErrorOutput    (unsigned int stack_err);

#endif
