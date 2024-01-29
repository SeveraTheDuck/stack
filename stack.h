#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define STACK_GET_ELEM_INFO __FILE__, __func__, __LINE__

#define STACK_CTOR_RECIVE_INFO const char*  const stack_init_name,\
                               const char*  const stack_init_file,\
                               const char*  const stack_init_func,\
                               const size_t       stack_init_line

#define StackCtor(stk, stack_elem_size) \
    StackConstructor ((stk), #stk, STACK_GET_ELEM_INFO, stack_elem_size)

#define StackDtor(stk) \
    StackDestructor ((stk))

const size_t STACK_DATA_ARRAY_INITIAL_CAPACITY     = 4;
const size_t STACK_DATA_ARRAY_EXPANSION_MULTIPLIER = 2;

const size_t STACK_MAX_SIZE    = __INT64_MAX__ / 2;
const size_t STACK_NULL_SIZE   = 0;
const int8_t STACK_POISON_BYTE = 0;

typedef size_t stack_error_t;

enum stack_errors
{
    NO_ERROR                    = 0,

    STACK_NULLPTR               = 1 << 0,
    STACK_DATA_NULLPTR          = 1 << 1,

    STACK_INVALID_SIZE          = 1 << 2,
    STACK_INVALID_CAPACITY      = 1 << 3,
    STACK_SIZE_OUT_OF_RANGE     = 1 << 4,

    STACK_POP_ELEM_OUT_OF_RANGE = 1 << 5
};

struct stack
{
    void*  data_array;
    size_t data_array_capacity;
    size_t data_array_size;

    size_t elem_size;

    stack_error_t error_status;

    const char* init_name;
    const char* init_file;
    const char* init_func;
    size_t      init_line;
};

stack_error_t
StackConstructor (stack* const stk, STACK_CTOR_RECIVE_INFO,
                  const size_t stack_elem_size);

stack_error_t
StackDestructor  (stack* const stk);

stack_error_t
StackVerify      (stack* const stk);

void
StackDump  (const stack* const stk);

stack_error_t
StackPush        (stack*      const stk, 
                  const void* const push_value);

stack_error_t
StackPop         (stack* const stk,
                  void* const dest_ptr);

void* const
StackGetElemPtrByIndex (stack* const stk,
                        const size_t elem_index);