#ifndef __STACK_ERRORS_H__
#define __STACK_ERRORS_H__

#include <stdarg.h>
#include "const.h"

Error_t StackDataPtrError (Stack* const stk);

Error_t StackIsAnyError   (ErrorType* stack_err);

Error_t StackVerify       (Stack* stk);

void StackDump            (Stack* stk, const char* file_name,
                                      const int line,
                                      const char* func_name);

void StackDumpHeader      (Stack* stk, const char* file_name,
                                      const int line,
                                      const char* func_name);

void StackDumpInfo        (Stack* stk);

void StackDumpData        (Stack* stk);

#ifdef CANARY_PROTECTION
    void StackDumpCanary  (Stack* stk);
#endif

#ifdef HASH_PROTECTION
    void StackDumpHash    (Stack* const stk);
#endif

void StackErrorOutput     (ErrorType* stack_err);

#endif
