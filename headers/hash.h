#ifndef __HASH_H__
#define __HASH_H__

#include <stdio.h>
#include "stack.h"
#include "errors.h"
#include "const.h"

#ifdef HASH_PROTECTION
    Error_t StackFindHash  (Stack* stk);

    Error_t StackHashError (Stack* stk);
#endif

#endif
