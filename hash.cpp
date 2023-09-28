#include "headers/hash.h"

#ifdef HASH_PROTECTION

Hash_t StackFindHash (Stack* stk)
{
    assert      (stk);
    STACKVERIFY (stk);

    char* ptr = (char*) stk;
    Hash_t hash = 0;
    size_t border =  sizeof (Stack) - sizeof (Hash_t);
#ifdef CANARY_PROTECTION
    border -= sizeof (Canary_t);
#endif

    for (size_t i = 0; i < border ; ++i)
    {
        hash += (Hash_t) *(ptr + i);
    }

#ifdef CANARY_PROTECTION
    size_t canary_begin  = sizeof (Stack) - sizeof (Canary_t);
    size_t canary_border = sizeof (Stack);
    for (size_t i = canary_begin; i < canary_border; ++i)
    {
        hash += (Hash_t) *(ptr + i);
    }
#endif

    ptr = (char*) stk->data;
    for (size_t i = 0; i < stk->data_capacity * sizeof (Elem_t); ++i)
    {
        hash += (Hash_t) *(ptr + i);
    }

#ifdef CANARY_PROTECTION
    char*  left_canary_ptr = (ptr - sizeof(Canary_t));
    char* right_canary_ptr = (ptr + stk->data_capacity * sizeof (Elem_t));
    for (size_t i = 0; i < sizeof (Canary_t); ++i)
    {
        hash += (Hash_t) *(left_canary_ptr  + i);
        hash += (Hash_t) *(right_canary_ptr + i);
    }
#endif

    return hash;
}

Error_t StackHashError (Stack* stk)
{
    assert (stk);

    Hash_t new_hash_value = StackFindHash (stk);
    if (stk->hash_value != new_hash_value)
    {
        if (stk->hash_value != SURPRISE_VALUE)
        {
            stk->stack_err.STACK_HASH_DAMAGED = 1;
        }
    }

    return stk->stack_err.STACK_HASH_DAMAGED;
}

#endif
