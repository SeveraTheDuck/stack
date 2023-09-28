#ifndef __STACK_H__
#define __STACK_H__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "config.h"
#include "const.h"

#define STACK_CTOR(stk) StackCtor ((stk), #stk,\
                                    __LINE__, __FILE__, __func__)

#define STACK_CTOR_CAP(stk, stack_capacity) StackCtor ((stk), #stk,\
                                                        __LINE__, __FILE__, __func__,\
                                                        stack_capacity)

#define STACKDUMP(stk) StackDump (stk, __FILE__, __LINE__, __func__)

#define STACKVERIFY(stk)\
    assert (stk);\
    (stk)->stack_err.STACK_ERROR_OCCURED = StackVerify (stk);\
    if ((stk)->stack_err.STACK_ERROR_OCCURED)\
    {\
        STACKDUMP(stk);\
        return (stk)->stack_err.STACK_ERROR_OCCURED;\
    }

Error_t StackCtor      (Stack* const stk,
                        const char*  const init_name,
                        const size_t       init_line,
                        const char*  const file_name,
                        const char*  const init_func,
                        size_t stack_capacity = INIT_CAPACITY);

Error_t StackDtor      (Stack* stk);

Error_t StackDataAlloc (Stack* const stk, Elem_t* const allocated_memory);

#ifdef CANARY_PROTECTION
    Error_t FillCanary (Stack* const stk);
#endif

Error_t StackResize    (Stack* const stk, const ResizeMode direction);

Error_t StackPush      (Stack* const stk, const Elem_t value);

Error_t StackPop       (Stack* const stk, Elem_t* const return_value);

Error_t StackPrint     (Stack* const stk);

#endif
