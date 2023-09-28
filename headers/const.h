#ifndef __CONST_H__
#define __CONST_H__

typedef unsigned long long Error_t;

#ifdef HASH_PROTECTION
    typedef unsigned long long Hash_t;
    const Hash_t SURPRISE_VALUE = 1234;
#endif

#ifdef CANARY_PROTECTION
    typedef unsigned long long Canary_t;
    #define CANARY_F "%llu"
    const Canary_t CANARY_VALUE = 31415926UL;
#endif

const Error_t STACK_DTOR_NO_ERROR = 0;

struct ErrorType
{
    Error_t STACK_ERROR_OCCURED             : 1;
    Error_t STACK_NULL_POINTER              : 1;
    Error_t STACK_DATA_NULL_POINTER         : 1;
    Error_t STACK_SIZE_NOT_LEGIT_VALUE      : 1;
    Error_t STACK_CAPACITY_NOT_LEGIT_VALUE  : 1;
    Error_t STACK_SIZE_OUT_OF_RANGE         : 1;

#ifdef CANARY_PROTECTION
    Error_t STACK_LEFT_CANARY_DAMAGED       : 1;
    Error_t STACK_RIGHT_CANARY_DAMAGED      : 1;
    Error_t STACK_DATA_LEFT_CANARY_DAMAGED  : 1;
    Error_t STACK_DATA_RIGHT_CANARY_DAMAGED : 1;
#endif

#ifdef HASH_PROTECTION
    Error_t STACK_HASH_DAMAGED              : 1;
#endif
};

struct Stack
{
#ifdef CANARY_PROTECTION
    Canary_t left_canary;
#endif

    Elem_t*  data;
    size_t   data_size;
    size_t   data_capacity;
    size_t   init_line;
    const char* init_name;
    const char* init_file;
    const char* init_func;

    ErrorType stack_err;

#ifdef HASH_PROTECTION
    Hash_t hash_value;
#endif

#ifdef CANARY_PROTECTION
    Canary_t right_canary;
#endif
};

enum ResizeMode
{
    SHORTEN = -1,
    EXPAND  = 1
};

#endif
