#ifndef __CONST_H__
#define __CONST_H__

/// @brief This typedef is used to use, mark and return errors.
typedef unsigned long long Error_t;

#ifdef HASH_PROTECTION

    /// @brief This typedef is used to use, mark and return hash value.
    typedef unsigned long long Hash_t;

#endif

#ifdef CANARY_PROTECTION

    /// @brief This typedef is used to use, mark and return canary values.
    typedef unsigned long long Canary_t;

    #define CANARY_F "%llu"

    /// @brief This is const value for canaries. If you try to change them, \
    /// it will return errors.
    const Canary_t CANARY_VALUE = 31415926UL;

#endif

/// @brief This const returns if destructor had no errors.
const Error_t STACK_DTOR_NO_ERROR = 0;

/// @brief This bit field is used to flag errors.
struct ErrorType
{
    Error_t STACK_ERROR_OCCURED                 : 1; ///< If errors have already occured. Returns to user.
    Error_t STACK_NULL_POINTER                  : 1; ///< If stack is nullptr.
    Error_t STACK_DATA_NULL_POINTER             : 1; ///< If stack data is nullptr.
    Error_t STACK_SIZE_NOT_LEGIT_VALUE          : 1; ///< If stack size is less than 0.
    Error_t STACK_CAPACITY_NOT_LEGIT_VALUE      : 1; ///< If stack capacity is less than 0.
    Error_t STACK_SIZE_OUT_OF_RANGE             : 1; ///< If stack size is greater than stack capacity.

    #ifdef CANARY_PROTECTION
        Error_t STACK_LEFT_CANARY_DAMAGED       : 1; ///< If left canary near stack struct is equal to CANARY_VALUE.
        Error_t STACK_RIGHT_CANARY_DAMAGED      : 1; ///< If right canary near stack struct is equal to CANARY_VALUE.
        Error_t STACK_DATA_LEFT_CANARY_DAMAGED  : 1; ///< If left canary near stack data is equal to CANARY_VALUE.
        Error_t STACK_DATA_RIGHT_CANARY_DAMAGED : 1; ///< If right canary near stack data is equal to CANARY_VALUE.
    #endif

    #ifdef HASH_PROTECTION
        Error_t STACK_HASH_DAMAGED              : 1; ///< If stack hash is not equal to new hash value.
    #endif
};

/// @brief This is the main struct with stack.
/// It contains stack data, its size and capacity, its initial info,
/// struct with errors, canaries and hash.
struct Stack
{
    #ifdef CANARY_PROTECTION
        Canary_t left_canary;
    #endif

    Elem_t*     data;
    size_t      data_size;
    size_t      data_capacity;
    size_t      init_line;
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

/// @brief This enum is used to say realloc function whether capacity is increased or decreased.
enum ResizeMode
{
    SHORTEN = -1, ///< Capacity is decreased.
    EXPAND  =  1  ///< Capacity is increased.
};

#endif
