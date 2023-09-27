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

#define STACKDUMP(stk) StackDump(stk, __FILE__, __LINE__, __func__)

#define STACKVERIFY(stk)\
    (stk)->stack_err.STACK_ERROR_OCCURED = StackVerify(stk);\
    if ((stk)->stack_err.STACK_ERROR_OCCURED)\
    {\
        STACKDUMP(stk);\
        return (stk)->stack_err.STACK_ERROR_OCCURED;\
    }

typedef unsigned long long Error_t;

typedef unsigned long long Canary_t;
#define CANARY_F "%llu"
const Canary_t CANARY_VALUE = 31415926UL;

struct ErrorType
{
    Error_t STACK_ERROR_OCCURED             : 1;
    Error_t STACK_NULL_POINTER              : 1;
    Error_t STACK_DATA_NULL_POINTER         : 1;
    Error_t STACK_SIZE_NOT_LEGIT_VALUE      : 1;
    Error_t STACK_CAPACITY_NOT_LEGIT_VALUE  : 1;
    Error_t STACK_SIZE_OUT_OF_RANGE         : 1;
    Error_t STACK_LEFT_CANARY_DAMAGED       : 1;
    Error_t STACK_RIGHT_CANARY_DAMAGED      : 1;
    Error_t STACK_DATA_LEFT_CANARY_DAMAGED  : 1;
    Error_t STACK_DATA_RIGHT_CANARY_DAMAGED : 1;
    Error_t STACK_HASH_DAMAGED              : 1;
};

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

    ErrorType stack_err;

    Canary_t right_canary;
};

enum ResizeMode
{
    SHORTEN = -1,
    EXPAND  = 1
};


Error_t StackCtor      (Stack* stk,
                        const char*  init_name,
                        const size_t init_line,
                        const char*  file_name,
                        const char*  init_func,
                        size_t stack_capacity = INIT_CAPACITY);

Error_t StackDtor      (Stack* stk);

Error_t StackDataAlloc (Stack* stk, Elem_t* allocated_memory);

Error_t FillCanary     (Stack* stk);

Error_t StackResize    (Stack* stk, ResizeMode direction);

Error_t StackPush      (Stack* stk, Elem_t value);

Error_t StackPop       (Stack* stk, Elem_t* return_value);

Error_t StackPrint     (Stack* stk);

Error_t StackVerify    (Stack* stk);

void StackDump           (Stack* stk, const char* file_name,
                                      const int line,
                                      const char* func_name);

void StackDumpHeader     (Stack* stk, const char* file_name,
                                      const int line,
                                      const char* func_name);

void StackDumpCanary     (Stack* stk);

void StackDumpInfo       (Stack* stk);

void StackDumpData       (Stack* stk);

void StackErrorOutput    (ErrorType* stack_err);

#endif
