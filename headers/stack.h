#ifndef __STACK_H__
#define __STACK_H__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "config.h"
#include "const.h"

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

#endif
