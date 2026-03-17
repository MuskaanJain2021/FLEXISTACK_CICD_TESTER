#ifndef STACKOPERATIONS_H
#define STACKOPERATIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// ERROR CODES
typedef enum
{
    STACK_CORRECT,      // No error, operation successful
    STACK_MEMORY_ERROR, // Memory allocation error
    STACK_UNDERFLOW,    // Attempt to pop from an empty stack
    STACK_EMPTY,        // Stack is empty (check operation)
    STACK_OVERFLOW      // Stack capacity exceeded (push operation)
} StackErrorCode;

// Different data types that can be stored in the stack memory
typedef enum
{
    TYPE_INT=0,    // Integer data type
    TYPE_FLOAT,  // Floating-point data type
    TYPE_DOUBLE, // Double precision floating-point data type
    TYPE_STRING  // String (character array) data type
} DataTypeIdentifier;

// Stack data
typedef union
{
    int intVal;
    float floatVal;
    double doubleVal;
    char *stringVal;
   // DataTypeIdentifier type; 
} StackData;

// Stack node structure
typedef struct StackNode
{
    DataTypeIdentifier type; // Type of data stored in the node
    StackData data;          // Union to store different data types
    struct StackNode *next;  // Pointer to the next node in the stack
} StackNode;

// Define a generic stack structure
typedef struct
{
    StackNode *top; // Pointer to the top of the stack
    int capacity;   // Capacity of the stack (optional, depending on implementation)
    int max_limit; // Maximum limit for stack size (set by ResizeStack)
} GenericStack;

#define DEFAULT_STACK_SIZE 10
// Typedef specific stack types
typedef GenericStack IntStack;
typedef GenericStack FloatStack;
typedef GenericStack DoubleStack;
typedef GenericStack StringStack;

// Function prototypes

/**
 * @brief Create a new stack with the specified capacity.
 *
 * @param capacity The initial capacity of the stack.
 * @return A pointer to the created stack, or NULL if memory allocation fails.
 */
GenericStack *CreateStack(int capacity);

/**
 * @brief Push data onto the stack.
 *
 * @param stack Pointer to the stack.
 * @param data The data to push onto the stack.
 * @param type The data type identifier.
 * @param error Pointer to StackErrorCode to store error status.
 */
void Push(GenericStack *stack, StackData data, DataTypeIdentifier type, StackErrorCode *error);

/**
 * @brief Pop data from the stack.
 *
 * @param stack Pointer to the stack.
 * @param error Pointer to StackErrorCode to store error status.
 * @param type Pointer to store the data type identifier of the popped data.
 * @return The popped data from the stack.
 */
StackData Pop(GenericStack *stack, StackErrorCode *error, DataTypeIdentifier *type);

/**
 * @brief Check if the stack is empty.
 *
 * @param stack Pointer to the stack.
 * @return true if the stack is empty, false otherwise.
 */
bool CheckEmpty(const GenericStack *stack);

/**
 * @brief Resize the stack to the new size.
 *
 * @param stack Pointer to the stack.
 * @param newSize The new size to resize the stack to.
 * @param error Pointer to StackErrorCode to store error status.
 */
void ResizeStack(GenericStack *stack, int newSize, StackErrorCode *error);

/**
 * @brief Delete the stack and free all allocated memory.
 *
 * @param stack Pointer to the stack to delete.
 */
void DeleteStack(GenericStack *stack);

/**
 * @brief Free all dynamically allocated memory in the string stack without deleting the stack structure itself.
 *
 * @param stack Pointer to the  string stack.
 */
void FreeStack(StringStack *stack);

/**
 * @brief Print the contents of the stack from bottom to top.
 *
 * @param top Pointer to the top node of the stack.
 */
void PrintStackBottemToTop(GenericStack *stack);
/**
 * @brief Calculate the length of a string (custom implementation of strlen).
 *
 * @param str The string to calculate the length of.
 * @return The length of the string.
 */
void *my_memcpy(void *restrict dest, const void *restrict src, size_t count);
size_t MY_StrLenCalc(const char *str);

/**
 * @brief Assign integer data to a stack node.
 *
 * @param node Pointer to the stack node.
 * @param data The integer data to assign.
 */
void assignIntData(StackNode *node, StackData data);

/**
 * @brief Assign float data to a stack node.
 *
 * @param node Pointer to the stack node.
 * @param data The float data to assign.
 */
void assignFloatData(StackNode *node, StackData data);

/**
 * @brief Assign double data to a stack node.
 *
 * @param node Pointer to the stack node.
 * @param data The double data to assign.
 */
void assignDoubleData(StackNode *node, StackData data);

/**
 * @brief Assign string data to a stack node.
 *
 * @param node Pointer to the stack node.
 * @param data The string data to assign.
 */
void assignStringData(StackNode *node, StackData data);

// Function pointer array for data assignment based on DataTypeIdentifier
extern void (*assignDataFunc[])(StackNode *, StackData);

// /**
//  * @brief Handle integer data.
//  *
//  * @param data The integer data to handle.
//  */
// void handleInt(StackData data);

// /**
//  * @brief Handle float data.
//  *
//  * @param data The float data to handle.
//  */
// void handleFloat(StackData data);

// /**
//  * @brief Handle double data.
//  *
//  * @param data The double data to handle.
//  */
// void handleDouble(StackData data);

// /**
//  * @brief Handle string data.
//  *
//  * @param data The string data to handle.
//  */
// void handleString(StackData data);

// Individual stack functions for specific data types

/**
 * @brief Create a new integer stack.
 *
 * @return Pointer to the created integer stack.
 */
IntStack *CreateIntStack();

/**
 * @brief Push integer data onto the integer stack.
 *
 * @param stack Pointer to the integer stack.
 * @param data The integer data to push.
 */
void PushInt(IntStack *stack, int data);

/**
 * @brief Pop integer data from the integer stack.
 *
 * @param stack Pointer to the integer stack.
 * @return The popped integer data.
 */
int PopInt(IntStack *stack);

/**
 * @brief Check if the integer stack is empty.
 *
 * @param stack Pointer to the integer stack.
 * @return true if the integer stack is empty, false otherwise.
 */
bool CheckEmptyInt(const IntStack *stack);

/**
 * @brief Delete the integer stack and free all allocated memory.
 *
 * @param stack Pointer to the integer stack to delete.
 */
void DeleteIntStack(IntStack *stack);

/**
 * @brief Create a new float stack.
 *
 * @return Pointer to the created float stack.
 *
 */

FloatStack *CreateFloatStack();

/**
 * @brief Push float data onto the float stack.
 *
 * @param stack Pointer to the float stack.
 * @param data The float data to push.
 */
void PushFloat(FloatStack *stack, float data);

/**
 * @brief Pop float data from the float stack.
 *
 * @param stack Pointer to the float stack.
 * @return The popped float data.
 */
float PopFloat(FloatStack *stack);

/**
 * @brief Check if the float stack is empty.
 *
 * @param stack Pointer to the float stack.
 * @return true if the float stack is empty, false otherwise.
 */
bool CheckEmptyFloat(const FloatStack *stack);

/**
 * @brief Delete the float stack and free all allocated memory.
 *
 * @param stack Pointer to the float stack to delete.
 */
void DeleteFloatStack(FloatStack *stack);

/**
 * @brief Create a new double stack.
 *
 * @return Pointer to the created double stack.
 */
DoubleStack *CreateDoubleStack();

/**
 * @brief Push double data onto the double stack.
 *
 * @param stack Pointer to the double stack.
 * @param data The double data to push.
 */
void PushDouble(DoubleStack *stack, double data);

/**
 * @brief Pop double data from the double stack.
 *
 * @param stack Pointer to the double stack.
 * @return The popped double data.
 */
double PopDouble(DoubleStack *stack);

/**
 * @brief Check if the double stack is empty.
 *
 * @param stack Pointer to the double stack.
 * @return true if the double stack is empty, false otherwise.
 */
bool CheckEmptyDouble(const DoubleStack *stack);

/**
 * @brief Delete the double stack and free all allocated memory.
 *
 * @param stack Pointer to the double stack to delete.
 */
void DeleteDoubleStack(DoubleStack *stack);

/**
 * @brief Create a new string stack.
 *
 * @return Pointer to the created string stack.
 */
StringStack *CreateStringStack();

/**
 * @brief Push string data onto the string stack.
 *
 * @param stack Pointer to the string stack.
 * @param data The string data to push.
 */
void PushString(StringStack *stack, const char *data);

/**
 * @brief Pop string data from the string stack.
 *
 * @param stack Pointer to the string stack.
 * @return The popped string data.
 */
char *PopString(StringStack *stack);

/**
 * @brief Check if the string stack is empty.
 *
 * @param stack Pointer to the string stack.
 * @return true if the string stack is empty, false otherwise.
 */
bool CheckEmptyString(const StringStack *stack);

/**
 * @brief Delete the string stack and free all allocated memory.
 *
 * @param stack Pointer to the string stack to delete.
 */
void DeleteStringStack(StringStack *stack);

#endif // STACKOPERATIONS_H