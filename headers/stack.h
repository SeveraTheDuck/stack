#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "config.h"
#include "const.h"
#include "errors.h"

/// @brief This define includes line number, file name and function name where STACK_CTOR was called.
/// It is used for STACK_CTOR() call.
#define STACK_GET_ELEM_INFO __LINE__, __FILE__, __func__

#ifdef _DEBUG
/// @brief This define includes initial name, line number, file name and function name where STACK_CTOR was called.
/// It is used for StackCtor() function arguments list.
    #define STACK_CTOR_RECIVE_INFO const char*  const init_name,\
                                   const size_t       init_line,\
                                   const char*  const init_file,\
                                   const char*  const init_func,
#endif

#ifdef _DEBUG
/// @brief This is a short call for StackCtor() function (debug version).
    #define STACK_CTOR(stk) StackCtor ((stk), #stk,\
                                        STACK_GET_ELEM_INFO)
#else
/// @brief This is a short call for StackCtor() function (not debug version).
/// Yeah I know its not shorter, go and prove Ferma's theoreme or smth.
    #define STACK_CTOR(stk) StackCtor ((stk))
#endif

#ifdef _DEBUG
/// @brief This is a short call for StackCtor() function with capacity parametr (debug version).
    #define STACK_CTOR_CAP(stk, stack_capacity) StackCtor ((stk), #stk,\
                                                            STACK_GET_ELEM_INFO,\
                                                            stack_capacity)
#else
/// @brief This is a short call for StackCtor() function with capacity parametr (not debug version).
    #define STACK_CTOR_CAP(stk, stack_capacity) StackCtor ((stk),\
                                                            stack_capacity)
#endif

/// @brief This is a short call for StackDtor() function.
/// It is not shorter, but used for sameness of macro calls for StackCtor() and StackDtor().
#define STACK_DTOR(stk) StackDtor (stk)

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

    #ifdef _DEBUG
        size_t      init_line;
        const char* init_name;
        const char* init_file;
        const char* init_func;
    #endif

    ErrorType stack_err;

    /// Don't move this hash, it must be in the end but before right canary.
    #ifdef HASH_PROTECTION
        Hash_t hash_value;
    #endif

    /// Don't move this canary, it must be in the end.
    #ifdef CANARY_PROTECTION
        Canary_t right_canary;
    #endif
};

/// @brief This is a constructor for stack struct.
/// @param stk Pointer to stack.
/// @param init_name Initial name of stack.
/// @param init_line Initial line of stack.
/// @param init_file Initial file of stack.
/// @param init_func Initial function of stack.
/// @param stack_capacity Initial capacity of stack. Optional parametr.
/// @return Struct stack_err with errors.
ErrorType StackCtor      (Stack* const stk,
                          #ifdef _DEBUG
                              STACK_CTOR_RECIVE_INFO
                          #endif
                          size_t stack_capacity = INIT_CAPACITY);

/// @brief This is a destructor for stack struct.
/// @param stk Pointer to stack.
/// @return Struct stack_err with errors.
ErrorType StackDtor      (Stack* stk);

/// @brief This is function for (re-)allocating memory for stack data.
/// @param stk Pointer to stack.
/// @param allocated_memory Pointer to previosly allocated memory. Nullptr in StackCtor().
/// @return Struct stack_err with errors.
ErrorType StackDataAlloc (Stack*  const stk,
                          Elem_t* const allocated_memory);

#ifdef CANARY_PROTECTION
/// @brief This is a function to fill canaries in stack data.
/// @param stk Pointer to stack.
/// @return Struct stack_err with errors.
    ErrorType FillCanary (Stack* const stk);
#endif

/// @brief This is a push function for stack.
/// @param stk Pointer to stack.
/// @param value Pushing value of type Elem_t.
/// @return Struct stack_err with errors.
ErrorType StackPush      (Stack* const stk, const Elem_t value);

/// @brief This is a pop function for stack.
/// @param stk Pointer to stack.
/// @param return_value Pointer to poping value of type Elem_t*.
/// @return Struct stack_err with errors.
ErrorType StackPop       (Stack*  const stk,
                          Elem_t* const return_value);

/// @brief This is a simple print of stack data.
/// @param stk Pointer to stack.
/// @return Struct stack_err with errors.
ErrorType StackPrint     (Stack* const stk);

#endif
