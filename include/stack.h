#pragma once



// for size_t only
#include <stddef.h>

// for particular implementation
#include "dynamic_array.h"


/**
 * @note
 * This file contains only stack interface, no particular implementation details
 * It is intended to be a way to use one interface for multiple implementations
 * Header files for particular implementations should be included in .c file
 *
 * For example, you may use dynamic array or linked list
 * for stack implementation, but the interface remains the same
 */



//-----------------------------------------------------------------------------
// Stack structure and names
//-----------------------------------------------------------------------------

/**
 * @brief Stack structure
 *
 * @details Contains pushed data and grants access via LIFO method
 * (Last In First Out)
 */
typedef struct stack stack;


/**
 * @brief Enumeration for stack error status codes
 */
enum stack_errors
{
    STACK_SUCCESS = 0,  ///< No error occurred, success
    STACK_ERROR   = 1   ///< Stack error occurred
};


/**
 * @brief A type to store stack error status
 */
typedef size_t stack_error_t;

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Stack interface
//-----------------------------------------------------------------------------

/**
 * @brief Constructor for stack structure
 *
 * @param elem_size Size of each stack element in bytes
 *
 * @retval Pointer to stack structure
 * @retval NULL if allocation error occurred
 *
 * @details Allocates memory for implementation defined stack structure
 */
stack*
StackConstructor (const size_t elem_size);


/**
 * @brief Destructor for stack structure
 *
 * @param stk Pointer to the stack
 *
 * @retval NULL
 *
 * @details Frees memory allocated for the stack structure
 * and its elements
 */
stack*
StackDestructor (stack* const stk);


/**
 * @brief Get stack top element
 *
 * @param stk Pointer to the stack
 * @param buffer Pointer to buffer where to copy the element
 *
 * @retval STACK_SUCCESS if stack element copied successfully
 * @retval STACK_ERROR   if no elements in stack or stack == NULL
 *
 * @details Copies stack top element into the buffer
 * @note make sure buffer is at least elem_size bytes long
 */
stack_error_t
StackTop (stack* const stk,
          void* const buffer);


/**
 * @brief Push element into stack
 *
 * @param stk Pointer to the stack
 * @param element Pointer to buffer to copy from
 *
 * @retval STACK_SUCCESS if element copied successfully
 * @retval STACK_ERROR   if error occured (nullptr or allocation)
 *
 * @details Copies element into stack and makes it top element
 */
stack_error_t
StackPush (stack* const stk,
           const void* const element);


/**
 * @brief Removes top element from stack
 *
 * @param stk Pointer to the stack
 *
 * @retval STACK_SUCCESS if top element removed successfully
 * @retval STACK_ERROR   if error occured (nullptr or allocation)
 *
 * @details Removes top element from stack and
 * makes the previous element top element
 */
stack_error_t
StackPop (stack* const stk);

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
