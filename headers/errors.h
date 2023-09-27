#ifndef __STACK_ERRORS_H__
#define __STACK_ERRORS_H__

#include <stdarg.h>
#include "const.h"

Error_t StackVerify       (Stack* stk);

void StackDump            (Stack* stk, const char* file_name,
                                      const int line,
                                      const char* func_name);

void StackDumpHeader      (Stack* stk, const char* file_name,
                                      const int line,
                                      const char* func_name);

void StackDumpCanary      (Stack* stk);

void StackDumpInfo        (Stack* stk);

void StackDumpData        (Stack* stk);

void StackErrorOutput     (ErrorType* stack_err);

#endif
