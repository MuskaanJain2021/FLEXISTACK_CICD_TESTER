/**
 * @file stack_operations.c
 * @brief Implementation of the FlexiStack library.
 * * This file contains the private data assignment functions and the
 * public API implementation for the generic stack.
 */

#include "stack_operations.h"
#include <stddef.h>
#include <string.h>

GenericStack GenericStackInstance = {NULL, 0, 0};
// Customized strlen function
size_t MY_StrLenCalc(const char *str)
{
    size_t len = 0;
    while (*str++)
    {
        len++;
    }
    return len;
}

void *my_memcpy(void *restrict dest, const void *restrict src, size_t count)
{
    // Cast the void pointers to char pointers for byte-wise copying
    char *d = (char *)dest;
    const char *s = (const char *)src;

    // Copy count bytes from src to dest
    for (size_t i = 0; i < count; ++i)
    {
        d[i] = s[i];
    }

    // Return the destination pointer
    return dest;
}

// Function pointer array for data assignment based on DataTypeIdentifier
void (*assignDataFunc[])(StackNode *, StackData) = {
    assignIntData,
    assignFloatData,
    assignDoubleData,
    assignStringData,
};

/**@INTERNAL */
// Assign data function for integer
void assignIntData(StackNode *node, StackData data)
{
    node->data.intVal = data.intVal;
}
/**@INTERNAL */
// Assign data function for float
void assignFloatData(StackNode *node, StackData data)
{
    node->data.floatVal = data.floatVal;
}
/**@INTERNAL */
// Assign data function for double
void assignDoubleData(StackNode *node, StackData data)
{
    node->data.doubleVal = data.doubleVal;
}
/**@INTERNAL */
// Assign data function for string
void assignStringData(StackNode *node, StackData data)
{
    size_t len = MY_StrLenCalc(data.stringVal);
    node->data.stringVal = (char *)calloc(len + 1, sizeof(char)); // Allocate memory for string
    if (node->data.stringVal != NULL)
    {
        my_memcpy(node->data.stringVal, data.stringVal, len + 1);
    }
    else
    {
        fprintf(stderr, "Memory allocation failed in assignStringData\n");
        return;
    }
}

GenericStack *CreateStack(int SIZE)
{
    GenericStack *stack = (GenericStack *)calloc(1, sizeof(GenericStack));
    if (stack == NULL)
        return NULL;
    stack->top = NULL;
    stack->capacity = 0;
    stack->max_limit = SIZE > 0 ? SIZE : 0; // Set max_limit to SIZE if it's greater than 0, otherwise set to 0 (indicating no limit)
    return stack;
}

void Push(GenericStack *stack, StackData data, DataTypeIdentifier type, StackErrorCode *error)
{

    StackErrorCode dummyerror;
    if (error == NULL)
        error = &dummyerror; // To prevent segmentation fault if caller passes NULL for error code, we use a dummy variable to store the error code locally in this case

    if (stack == NULL)
    {
        if (error)
            *error = STACK_MEMORY_ERROR;
        return;
    }

    if (stack->max_limit > 0 && stack->capacity >= stack->max_limit)
    {
        if (error)
            *error = STACK_OVERFLOW;
        return;
    }

    if (type < TYPE_INT || type > TYPE_STRING)
    {
        if (error)
            *error = STACK_MEMORY_ERROR; // Or a custom STACK_TYPE_ERROR
        return;
    }

    // Allocation and node creation should be done before modifying the stack to avoid leaving it in an inconsistent state on allocation failure
    //  Create a new node
    StackNode *newNode = (StackNode *)calloc(1, sizeof(StackNode));
    if (newNode == NULL)
    {
        *error = STACK_MEMORY_ERROR;
        return;
    }

    // Assign data and type to the new node
    newNode->type = type;
    assignDataFunc[type](newNode, data);

    if (type == TYPE_STRING && newNode->data.stringVal == NULL)
    {
        free(newNode);
        *error = STACK_MEMORY_ERROR;
        return;
    }
    // Insert the new node at the top of the stack
    newNode->next = stack->top;
    stack->top = newNode;
    stack->capacity++;
    *error = STACK_CORRECT;
}

StackData Pop(GenericStack *stack, StackErrorCode *error, DataTypeIdentifier *type)
{
    StackData data = {0};
    if (stack == NULL || stack->top == NULL)
    {
        *error = stack == NULL ? STACK_MEMORY_ERROR : STACK_UNDERFLOW;
        return data; // Return empty data on error
    }

    // Remove the top node from the stack
    StackNode *temp = stack->top;
    if (type != NULL)
        *type = temp->type; // Set the type of the popped data if type pointer is provided
    //*type = temp->type;

    if (temp->type == TYPE_INT)
    {
        data.intVal = temp->data.intVal;
    }
    else if (temp->type == TYPE_FLOAT)
    {
        data.floatVal = temp->data.floatVal;
    }
    else if (temp->type == TYPE_DOUBLE)
    {
        data.doubleVal = temp->data.doubleVal;
    }
    else if (temp->type == TYPE_STRING)
    {
        data.stringVal = temp->data.stringVal;
    }
    else
    {
        *error = STACK_MEMORY_ERROR;
        return data; // Return empty data on error
    }
    stack->top = stack->top->next;
    free(temp); // Free the node itself, not the string memory if TYPE_STRING

    stack->capacity--;

    *error = STACK_CORRECT;
    return data;
}

bool CheckEmpty(const GenericStack *stack)
{
    return stack == NULL || stack->top == NULL;
}

void DeleteStack(GenericStack *stack)
{
    if (stack != NULL)
    {
        StackNode *current = stack->top;
        while (current != NULL)
        {
            StackNode *tmp = current;
            current = current->next;
            if (tmp->type == TYPE_STRING && tmp->data.stringVal != NULL)
            {
                free(tmp->data.stringVal);
            }

            free(tmp);
        }
        free(stack);
    }
}

void ResizeStack(GenericStack *stack, int newSize, StackErrorCode *error)
{
    if (error != NULL)
        *error = STACK_CORRECT; // Initialize error code to correct at the beginning

    if (stack == NULL || newSize <= 0)
    {
        if (error)
            *error = STACK_MEMORY_ERROR;
        return;
    }

    // keep popping until we reduce the stack to the new size if current capacity exceeds new size
    while (stack->capacity > newSize)
    {
        DataTypeIdentifier discardType;
        StackData DiscardedData = Pop(stack, error, &discardType);
        if (*error != STACK_CORRECT)
            break; // Stop popping if an error occurs

        if (discardType == TYPE_STRING && DiscardedData.stringVal != NULL)
        {
            free(DiscardedData.stringVal); // Free the string memory if it's a string type
        }
    }
    stack->max_limit = newSize;

    *error = STACK_CORRECT;
}

void PrintStackBottemToTop(GenericStack *stack)
{
    if (stack == NULL || stack->top == NULL)
    {
        printf("Stack is empty.\n");
        return;
    }
    // PrintStackBottemToTop(top->next);
    // Allocate a temp array of pointers on heap to store the nodes for reverse printing
    StackNode **nodeArr = (StackNode **)malloc(stack->capacity * sizeof(StackNode *));

    if (nodeArr == NULL)
    {
        fprintf(stderr, "Memory allocation failed in PrintStackBottemToTop\n");
        return;
    }

    // Go and Fill the array with node pointers (top to bottem)
    StackNode *current = stack->top;
    int idx = 0;
    while (current != NULL && idx < stack->capacity)
    {
        nodeArr[idx++] = current;
        current = current->next;
    }

    // Print array backward (bottem to top)
    for (int i = idx - 1; i >= 0; i--)
    {
        const StackNode *node = nodeArr[i];
        if (node->type == TYPE_INT)
        {
            printf("%d ", node->data.intVal);
        }
        else if (node->type == TYPE_FLOAT)
        {
            printf("%f ", node->data.floatVal);
        }
        else if (node->type == TYPE_DOUBLE)
        {
            printf("%lf ", node->data.doubleVal);
        }
        else if (node->type == TYPE_STRING)
        {
            printf("%s ", node->data.stringVal);
        }
        else
        {
            fprintf(stderr, "Unknown data type in stack.\n");
        }
    }
    printf("\n");

    // free temp pointer array not the nodes themselves
    free(nodeArr);
}

// Individual stack functions

// Integer Stack functions
IntStack *CreateIntStack()
{
    return (IntStack *)CreateStack(DEFAULT_STACK_SIZE);
}

void PushInt(IntStack *stack, int data)
{
    StackData stackData;
    stackData.intVal = data;

    StackErrorCode error;
    Push((IntStack *)stack, stackData, TYPE_INT, &error);
    if (error != STACK_CORRECT)
    {
        fprintf(stderr, "Push failed for integer stack\n");
    }
}

int PopInt(IntStack *stack)
{
    StackErrorCode error;
    DataTypeIdentifier type;
    StackData data = Pop((IntStack *)stack, &error, &type);
    if (error != STACK_CORRECT || type != TYPE_INT)
    {
        fprintf(stderr, "Pop failed for integer stack\n");
        return 0; // Return 0 or some error code indicating failure
    }
    return data.intVal;
}

bool CheckEmptyInt(const IntStack *stack)
{
    return CheckEmpty((const IntStack *)stack);
}

void DeleteIntStack(IntStack *stack)
{
    DeleteStack((IntStack *)stack);
}

// Float Stack functions
FloatStack *CreateFloatStack()
{
    return (FloatStack *)CreateStack(DEFAULT_STACK_SIZE);
}

void PushFloat(FloatStack *stack, float data)
{
    StackData stackData;
    stackData.floatVal = data;
    StackErrorCode error;
    Push((FloatStack *)stack, stackData, TYPE_FLOAT, &error);
    if (error != STACK_CORRECT)
    {
        fprintf(stderr, "Push failed for float stack\n");
    }
}

float PopFloat(FloatStack *stack)
{
    StackErrorCode error;
    DataTypeIdentifier type;
    StackData data = Pop((FloatStack *)stack, &error, &type);
    if (error != STACK_CORRECT || type != TYPE_FLOAT)
    {
        fprintf(stderr, "Pop failed for float stack\n");
        return 0.0; // Return 0.0 or some error code indicating failure
    }
    return data.floatVal;
}

bool CheckEmptyFloat(const FloatStack *stack)
{
    return CheckEmpty((const FloatStack *)stack);
}

void DeleteFloatStack(FloatStack *stack)
{
    DeleteStack((FloatStack *)stack);
}

// Double Stack functions
DoubleStack *CreateDoubleStack()
{
    return (DoubleStack *)CreateStack(DEFAULT_STACK_SIZE);
}

void PushDouble(DoubleStack *stack, double data)
{
    StackData stackData;
    stackData.doubleVal = data;
    StackErrorCode error;
    Push((DoubleStack *)stack, stackData, TYPE_DOUBLE, &error);
    if (error != STACK_CORRECT)
    {
        fprintf(stderr, "Push failed for double stack\n");
    }
}

double PopDouble(DoubleStack *stack)
{
    StackErrorCode error;
    DataTypeIdentifier type;
    StackData data = Pop((DoubleStack *)stack, &error, &type);
    if (error != STACK_CORRECT || type != TYPE_DOUBLE)
    {
        fprintf(stderr, "Pop failed for double stack\n");
        return 0.0; // Return 0.0 or some error code indicating failure
    }
    return data.doubleVal;
}

bool CheckEmptyDouble(const DoubleStack *stack)
{
    return CheckEmpty((const DoubleStack *)stack);
}

void DeleteDoubleStack(DoubleStack *stack)
{
    DeleteStack((DoubleStack *)stack);
}

// String Stack functions
StringStack *CreateStringStack()
{
    return (StringStack *)CreateStack(DEFAULT_STACK_SIZE);
}

// PushString function using memcpy
void PushString(StringStack *stack, const char *data)
{
    StackData stackData;
    StackErrorCode error;
    stackData.stringVal = (char *)data;
    if (stackData.stringVal != NULL)
    {
        Push((StringStack *)stack, stackData, TYPE_STRING, &error);
        if (error != STACK_CORRECT)
        {
            fprintf(stderr, "Push failed for string stack\n");
        }
    }
    else
    {
        fprintf(stderr, "Memory allocation failed in PushString\n");
    }
}

char *PopString(StringStack *stack)
{
    StackErrorCode error;
    DataTypeIdentifier type;
    StackData data = Pop((StringStack *)stack, &error, &type);
    if (error != STACK_CORRECT)
    {
        fprintf(stderr, "Pop failed with the code %d\n", error);
        return NULL; // Return NULL
    }

    if (type != TYPE_STRING)
    {
        fprintf(stderr, "Type mismatch: Expected string type but got type %d\n", type);
        return NULL; // Return NULL on type mismatch
    }
    // data.stringVal contains the address, we return it directly
    return data.stringVal;
}

bool CheckEmptyString(const StringStack *stack)
{
    return CheckEmpty((const StringStack *)stack);
}

void DeleteStringStack(StringStack *stack)
{
    DeleteStack((StringStack *)stack);
}

void FreeStack(StringStack *stack)
{
    if (stack == NULL)
        return;

    StackNode *current = stack->top;
    StackNode *next;

    while (current != NULL)
    {
        next = current->next;

        // Free  dynamically allocated string data
        if (current->type == TYPE_STRING)
        {
            free(current->data.stringVal);
        }

        free(current); // Free the node itself
        current = next;
    }
}