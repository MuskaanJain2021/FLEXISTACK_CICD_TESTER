#include "unity.h"

// Link to functions in test_flexistack.c
extern void test_MultipleDataTypeIntegration(void);
extern void test_StringStack_DeepCopy(void);
extern void test_Stack_Resize_Shrink_Cleanup(void);


int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_MultipleDataTypeIntegration);
    RUN_TEST(test_StringStack_DeepCopy);
    RUN_TEST(test_Stack_Resize_Shrink_Cleanup);
    return UNITY_END();
}