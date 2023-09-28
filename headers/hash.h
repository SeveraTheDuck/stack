#include <stdio.h>
#include "config.h"

#ifdef HASH_PROTECTION
    #ifndef __HASH_H__
    #define __HASH_H__

    #include "stack.h"
    #include "errors.h"
    #include "const.h"

    Error_t StackFindHash  (Stack* stk);

    Error_t StackHashError (Stack* stk);

    #endif
#endif
