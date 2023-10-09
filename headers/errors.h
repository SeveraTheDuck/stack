#ifndef STACK_ERRORS_H
#define STACK_ERRORS_H

#include <stdarg.h>
#include "const.h"

#ifdef _DEBUG
/// @brief This define is short call for StackDump() function (debug version).
/// @param stk Pointer to stack.
    #define STACK_DUMP(stk) StackDump (stk, STACK_GET_ELEM_INFO)

/// @brief This define is short variant of list of arguments for StackDump().
    #define STACK_DUMP_RECIEVE_INFO const int   line,\
                                    const char* file_name,\
                                    const char* func_name
#else
/// @brief This define is short call for StackDump() function (not debug version).
    #define STACK_DUMP(stk) StackDump (stk)
#endif

/// @brief This define is a wrapper for StackVerify(). It also calls for STACK_DUMP().
/// @param stk Pointer to stack.
#define STACK_VERIFY(stk)\
    assert (stk);\
    (stk)->stack_err.STACK_ERROR_OCCURED = StackVerify (stk);\
    if ((stk)->stack_err.STACK_ERROR_OCCURED)\
    {\
        STACK_DUMP(stk);\
        return (stk)->stack_err;\
    }

/// @brief This function checks if pointer to stack data is nullptr.
/// @param stk Pointer to stack.
/// @return If data is nullptr, return STACK_DATA_NULL_POINTER, else return 0.
Error_t StackDataPtrError (Stack* const stk);

/// @brief This function checks if there were any errors by far.
/// @param stack_err Struct with errors.
/// @return Value of STACK_ERROR_OCCURED.
Error_t StackIsAnyError   (ErrorType* const stack_err);

/// @brief This is a verificator for stack. It checks whether it is legit or not.
/// @param stk Pointer to stack.
/// @return Calls for StackIsAnyError() and returns its value.
Error_t StackVerify       (Stack* const stk);

/// @brief This function prints output for non-legit stack.
/// @param stk Pointer to stack.
/// @param line Line where StackDump() was called.
/// @param file_name File where StackDump() was called.
/// @param func_name Function where StackDump was called.
/// You can use this function just to print information of your stack.
/// Call it within STACK_DUMP() macro.
void StackDump            (const Stack* const stk
                           #ifdef _DEBUG
                            , STACK_DUMP_RECIEVE_INFO
                           #endif
                          );

#ifdef _DEBUG
/// @brief This function prints header for StackDump().
/// @param stk Pointer to stack.
/// @param line Line where StackDump() was called.
/// @param file_name File where StackDump() was called.
/// @param func_name Function where StackDump was called.
/// It prints pointer value for stack, its name, function where it was created,
/// creation line, creation function, StackDump() call file and function.
    void StackDumpHeader  (const Stack* const stk,
                           STACK_DUMP_RECIEVE_INFO);
#endif

/// @brief This function prints size and capacity of stack.
/// @param stk Pointer to stack.
/// If data is not nullptr, calls for StackDumpData.
void StackDumpInfo        (const Stack* const stk);

/// @brief This function prints data of stack.
/// @param stk Pointer to stack.
void StackDumpData        (const Stack* const stk);

#ifdef CANARY_PROTECTION
/// @brief This function prints canaries which are located around stack and data.
/// @param stk Pointer to stack.
    void StackDumpCanary  (const Stack* const stk);
#endif

#ifdef HASH_PROTECTION
/// @brief This function prints hash value of stack.
/// @param stk Pointer to stack.
    void StackDumpHash    (const Stack* const stk);
#endif

/// @brief This function prints errors occured in stack.
/// @param stk Pointer to stack.
void StackErrorOutput     (const ErrorType* const stack_err);

#endif
