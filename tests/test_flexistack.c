#include "unity.h"
#include "stack_operations.h"

GenericStack* stack;

void setUp(void) {
    stack = CreateStack(10);
    stack->max_limit = 10;
}

void tearDown(void) {
    DeleteStack(stack);
}

void test_MultipleDataTypeIntegration(void) {
    StackData data;
    StackErrorCode err;
    DataTypeIdentifier type;

    // Push different types
    data.intVal = 42;
    Push(stack, data, TYPE_INT, &err);
    TEST_ASSERT_EQUAL_INT(STACK_CORRECT, err);

    data.floatVal = 3.14f;
    Push(stack, data, TYPE_FLOAT, &err);
    TEST_ASSERT_EQUAL_INT(STACK_CORRECT, err);

    // Test Resize (Lifting the ceiling)
    ResizeStack(stack, 20, &err);
    TEST_ASSERT_EQUAL_INT(STACK_CORRECT, err);
    TEST_ASSERT_EQUAL_INT(20, stack->max_limit);

    // Verify LIFO Order and Data Integrity
    StackData popped;
    popped = Pop(stack, &err, &type);
    TEST_ASSERT_EQUAL_INT(TYPE_FLOAT, type);
    TEST_ASSERT_EQUAL_FLOAT(3.14f, popped.floatVal);

    popped = Pop(stack, &err, &type);
    TEST_ASSERT_EQUAL_INT(TYPE_INT, type);
    TEST_ASSERT_EQUAL_INT(42, popped.intVal);
}

void test_StringStack_DeepCopy(void) {
    StringStack* sStack = CreateStringStack();
    sStack->max_limit = 5;
    
    char input[] = "Unity";
    PushString(sStack, input);
    
    // Modify original - stack should be unaffected
    input[0] = 'B'; 
    
    char* output = PopString(sStack);
    TEST_ASSERT_EQUAL_STRING("Unity", output);
    
    free(output); // Transfer of ownership
    DeleteStringStack(sStack);
}

void test_Stack_Resize_Shrink_Cleanup(void) {
    StackErrorCode err;
    PushInt(stack, 1);
    PushInt(stack, 2);
    PushInt(stack, 3);

    // Shrink stack to 1 - this should discard '3' and '2'
    ResizeStack(stack, 1, &err);
    
    TEST_ASSERT_EQUAL_INT(STACK_CORRECT, err);
    TEST_ASSERT_EQUAL_INT(1, stack->capacity);
    TEST_ASSERT_EQUAL_INT(1, PopInt(stack));
}

void test_Pop_Should_Handle_Null_Type_Pointer(void) {
    StackErrorCode err;
    PushInt(stack, 100);
    
    // Passing NULL for type should be safe now with our fix
    StackData d = Pop(stack, &err, NULL); 
    
    TEST_ASSERT_EQUAL_INT(STACK_CORRECT, err);
    TEST_ASSERT_EQUAL_INT(100, d.intVal);
}