#include "headers/FieldInfo.h"
#include "headers/DynamicArray.h"
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void test_createDynamicArray_with_data_capacity() {
    printf("--- Testing createDynamicArray with data_capacity ---\n");

    const FieldInfo *stringFieldInfo = getStringFieldInfo();
    DynamicArray *dynamicArray = createDynamicArray(3, stringFieldInfo);

    assert(dynamicArray != NULL && "createDynamicArray should return non-null pointer");
    assert(dynamicArray->data_capacity == 3 && "createDynamicArray should set correct data_capacity");
    assert(dynamicArray->size == 0 && "createDynamicArray should initialize size to 0");
    assert(dynamicArray->field_info == stringFieldInfo && "createDynamicArray should set correct field info");

    deleteDynamicArray(dynamicArray);
    printf("PASS: createDynamicArray with data_capacity\n");
}
void test_createEmptyDynamicArray() {
    printf("--- Testing createEmptyDynamicArray ---\n");

    const FieldInfo *stringFieldInfo = getStringFieldInfo();
    DynamicArray *dynamicArray = createEmptyDynamicArray(stringFieldInfo);

    assert(dynamicArray != NULL && "createEmptyDynamicArray should return non-null pointer");
    assert(dynamicArray->data_capacity == START_DATA_CAPACITY && "createEmptyDynamicArray should set data_capacity to START_DATA_CAPACITY constant");
    assert(dynamicArray->size == 0 && "createEmptyDynamicArray should initialize size to 0");
    assert(dynamicArray->field_info == stringFieldInfo && "createEmptyDynamicArray should set correct field info");

    deleteDynamicArray(dynamicArray);
    printf("PASS: createEmptyDynamicArray\n");
}
void test_add_and_get() {
    printf("--- Testing add and get ---\n");

    const FieldInfo *stringFieldInfo = getStringFieldInfo();
    DynamicArray *dynamicArray = createEmptyDynamicArray(stringFieldInfo);

    char str1[] = "Hello";
    char str2[] = "World";
    char str3[] = "Test";

    add(dynamicArray, str1);
    add(dynamicArray, str2);
    add(dynamicArray, str3);

    assert(dynamicArray->size == 3 && "Size should be 3 after adding 3 elements");
    assert(strcmp((char*)get(dynamicArray, 0), "Hello") == 0 && "First element should be 'Hello'");
    assert(strcmp((char*)get(dynamicArray, 1), "World") == 0 && "Second element should be 'World'");
    assert(strcmp((char*)get(dynamicArray, 2), "Test") == 0 && "Third element should be 'Test'");

    deleteDynamicArray(dynamicArray);
    printf("PASS: add and get\n");
}
void test_get_out_of_bounds() {
    printf("--- Testing get with out of bounds index ---\n");

    const FieldInfo *stringFieldInfo = getStringFieldInfo();
    DynamicArray *dynamicArray = createEmptyDynamicArray(stringFieldInfo);

    assert(get(dynamicArray, 0) == NULL && "get should return NULL for empty dynamicArray");
    assert(get(dynamicArray, -1) == NULL && "get should return NULL for negative index");
    assert(get(dynamicArray, 1) == NULL && "get should return NULL for out of bounds index");

    deleteDynamicArray(dynamicArray);
    printf("PASS: get out of bounds\n");
}
void *toUpper(void *element) {
    char *str = (char*)element;
    int len = strlen(str);
    char *upper = (char*)malloc(len + 1);
    for (int i = 0; i < len; i++) {
        upper[i] = toupper(str[i]);
    }
    upper[len] = '\0';
    return upper;
}
void test_map() {
    printf("--- Testing map ---\n");

    const FieldInfo *stringFieldInfo = getStringFieldInfo();
    DynamicArray *dynamicArray = createEmptyDynamicArray(stringFieldInfo);

    char str1[] = "hello";
    char str2[] = "world";

    add(dynamicArray, str1);
    add(dynamicArray, str2);

    map(dynamicArray, toUpper);

    assert(strcmp((char*)get(dynamicArray, 0), "HELLO") == 0 && "After map: First element should be 'HELLO'");
    assert(strcmp((char*)get(dynamicArray, 1), "WORLD") == 0 && "After map: Second element should be 'WORLD'");

    deleteDynamicArray(dynamicArray);
    printf("PASS: map\n");
}
bool startsWithT(const void *element) {
    const char *str = (const char*)element;
    return str[0] == 'T' ? true : false;
}
void test_where() {
    printf("--- Testing where ---\n");

    const FieldInfo *stringFieldInfo = getStringFieldInfo();
    DynamicArray *dynamicArray = createEmptyDynamicArray(stringFieldInfo);

    char str1[] = "Test";
    char str2[] = "Hello";
    char str3[] = "Today";
    char str4[] = "World";

    add(dynamicArray, str1);
    add(dynamicArray, str2);
    add(dynamicArray, str3);
    add(dynamicArray, str4);

    assert(dynamicArray->size == 4 && "Before where: Size should be 4");

    where(dynamicArray, startsWithT);

    assert(dynamicArray->size == 2 && "After where: Size should be 2 (only strings starting with 'T')");
    assert(strcmp((char*)get(dynamicArray, 0), "Test") == 0 && "After where: First element should be 'Test'");
    assert(strcmp((char*)get(dynamicArray, 1), "Today") == 0 && "After where: Second element should be 'Today'");

    deleteDynamicArray(dynamicArray);
    printf("PASS: where\n");
}
void test_concat() {
    printf("--- Testing concat ---\n");

    const FieldInfo *stringFieldInfo = getStringFieldInfo();
    DynamicArray *dynamicArray1 = createEmptyDynamicArray(stringFieldInfo);
    DynamicArray *dynamicArray2 = createEmptyDynamicArray(stringFieldInfo);

    char str1[] = "Hello";
    char str2[] = "World";
    char str3[] = "Test";
    char str4[] = "Concat";

    add(dynamicArray1, str1);
    add(dynamicArray1, str2);

    add(dynamicArray2, str3);
    add(dynamicArray2, str4);

    DynamicArray *concatenated = concat(dynamicArray1, dynamicArray2);

    assert(concatenated != NULL && "concat should return non-null pointer");
    assert(concatenated->size == 4 && "concatenated dynamicArray should have size 4");
    assert(strcmp((char*)get(concatenated, 0), "Hello") == 0 && "First element of concatenated dynamicArray should be 'Hello'");
    assert(strcmp((char*)get(concatenated, 1), "World") == 0 && "Second element of concatenated dynamicArray should be 'World'");
    assert(strcmp((char*)get(concatenated, 2), "Test") == 0 && "Third element of concatenated dynamicArray should be 'Test'");
    assert(strcmp((char*)get(concatenated, 3), "Concat") == 0 && "Fourth element of concatenated dynamicArray should be 'Concat'");

    deleteDynamicArray(dynamicArray1);
    deleteDynamicArray(dynamicArray2);
    deleteDynamicArray(concatenated);
    printf("PASS: concat\n");
}
void test_sort() {
    printf("--- Testing sort ---\n");

    const FieldInfo *stringFieldInfo = getStringFieldInfo();
    DynamicArray *dynamicArray = createEmptyDynamicArray(stringFieldInfo);

    char str1[] = "zebra";
    char str2[] = "apple";
    char str3[] = "banana";

    add(dynamicArray, str1);
    add(dynamicArray, str2);
    add(dynamicArray, str3);

    assert(dynamicArray->size == 3 && "Before sort: Size should be 3");

    sort(dynamicArray);

    assert(strcmp((char*)get(dynamicArray, 0), "apple") == 0 && "After sort: First element should be 'apple'");
    assert(strcmp((char*)get(dynamicArray, 1), "banana") == 0 && "After sort: Second element should be 'banana'");
    assert(strcmp((char*)get(dynamicArray, 2), "zebra") == 0 && "After sort: Third element should be 'zebra'");

    deleteDynamicArray(dynamicArray);
    printf("PASS: sort\n");
}
void test_deleteDynamicArray() {
    printf("--- Testing deleteDynamicArray ---\n");

    const FieldInfo *stringFieldInfo = getStringFieldInfo();
    DynamicArray *dynamicArray = createEmptyDynamicArray(stringFieldInfo);

    char str1[] = "ToDelete";
    char str2[] = "AlsoToDelete";

    add(dynamicArray, str1);
    add(dynamicArray, str2);

    assert(dynamicArray->size == 2 && "Size should be 2 before deletion");

    deleteDynamicArray(dynamicArray);

    printf("PASS: deleteDynamicArray\n");
}
void test_edge_cases() {
    printf("--- Testing edge cases ---\n");

    const FieldInfo *stringFieldInfo = getStringFieldInfo();

    DynamicArray *nullDynamicArray = NULL;
    assert(get(nullDynamicArray, 0) == NULL && "get on NULL dynamicArray should return NULL");

    DynamicArray *dynamicArray = createEmptyDynamicArray(stringFieldInfo);

    add(dynamicArray, NULL);
    assert(dynamicArray->size == 0 && "Adding NULL element should not increase size");

    DynamicArray *dynamicArrayFromNullField = createEmptyDynamicArray(NULL);
    assert(dynamicArrayFromNullField == NULL && "createEmptyDynamicArray with NULL field_info should return NULL");

    deleteDynamicArray(dynamicArray);
    printf("PASS: edge cases\n");
}
void test_data_capacity_extension() {
    printf("--- Testing data_capacity extension ---\n");

    const FieldInfo *stringFieldInfo = getStringFieldInfo();
    DynamicArray *dynamicArray = createDynamicArray(2, stringFieldInfo);

    char str1[] = "First";
    char str2[] = "Second";
    char str3[] = "Third";
    char str4[] = "Fourth";
    char str5[] = "Fifth";

    assert(dynamicArray->data_capacity == 2 && "Initial data_capacity should be 2");
    assert(dynamicArray->size == 0 && "Initial size should be 0");

    add(dynamicArray, str1);
    assert(dynamicArray->data_capacity == 2 && "data_capacity should remain 2 after adding first element");
    assert(dynamicArray->size == 1 && "Size should be 1 after adding first element");

    add(dynamicArray, str2);
    assert(dynamicArray->data_capacity == 2 && "data_capacity should remain 2 after adding second element");
    assert(dynamicArray->size == 2 && "Size should be 2 after adding second element");

    add(dynamicArray, str3);
    assert(dynamicArray->data_capacity == 4 && "data_capacity should double to 4 after exceeding initial data_capacity");
    assert(dynamicArray->size == 3 && "Size should be 3 after adding third element");

    add(dynamicArray, str4);
    assert(dynamicArray->data_capacity == 4 && "data_capacity should remain 4 after adding fourth element");
    assert(dynamicArray->size == 4 && "Size should be 4 after adding fourth element");

    add(dynamicArray, str5);
    assert(dynamicArray->data_capacity == 8 && "data_capacity should double to 8 after exceeding extended data_capacity");
    assert(dynamicArray->size == 5 && "Size should be 5 after adding fifth element");

    assert(strcmp((char*)get(dynamicArray, 0), "First") == 0 && "First element should be 'First'");
    assert(strcmp((char*)get(dynamicArray, 1), "Second") == 0 && "Second element should be 'Second'");
    assert(strcmp((char*)get(dynamicArray, 2), "Third") == 0 && "Third element should be 'Third'");
    assert(strcmp((char*)get(dynamicArray, 3), "Fourth") == 0 && "Fourth element should be 'Fourth'");
    assert(strcmp((char*)get(dynamicArray, 4), "Fifth") == 0 && "Fifth element should be 'Fifth'");

    deleteDynamicArray(dynamicArray);
    printf("PASS: data_capacity extension\n");
}
void test_double_operations() {
    printf("--- Testing operations with double values ---\n");

    const FieldInfo *doubleFieldInfo = getDoubleFieldInfo();
    DynamicArray *dynamicArray = createEmptyDynamicArray(doubleFieldInfo);

    double val1 = 3.14;
    double val2 = 2.71;
    double val3 = 1.41;

    add(dynamicArray, &val1);
    add(dynamicArray, &val2);
    add(dynamicArray, &val3);

    assert(dynamicArray->size == 3 && "Size should be 3 after adding 3 doubles");

    double *retrieved_val1 = (double*)get(dynamicArray, 0);
    double *retrieved_val2 = (double*)get(dynamicArray, 1);
    double *retrieved_val3 = (double*)get(dynamicArray, 2);

    assert(fabs(*retrieved_val1 - 3.14) < 1e-9 && "First element should be 3.14");
    assert(fabs(*retrieved_val2 - 2.71) < 1e-9 && "Second element should be 2.71");
    assert(fabs(*retrieved_val3 - 1.41) < 1e-9 && "Third element should be 1.41");


    double val4 = 0.5;
    double val5 = 5.0;
    add(dynamicArray, &val4);
    add(dynamicArray, &val5);

    sort(dynamicArray);

    double *sortedVal0 = (double*)get(dynamicArray, 0);
    double *sortedVal1 = (double*)get(dynamicArray, 1);
    double *sortedVal2 = (double*)get(dynamicArray, 2);
    double *sortedVal3 = (double*)get(dynamicArray, 3);
    double *sortedVal4 = (double*)get(dynamicArray, 4);

    assert(fabs(*sortedVal0 - 0.5) < 1e-9 && "First sorted element should be 0.5");
    assert(fabs(*sortedVal1 - 1.41) < 1e-9 && "Second sorted element should be 1.41");
    assert(fabs(*sortedVal2 - 2.71) < 1e-9 && "Third sorted element should be 3.14");
    assert(fabs(*sortedVal3 - 3.14) < 1e-9 && "Fourth sorted element should be 5.0");
    assert(fabs(*sortedVal4 - 5) < 1e-9 && "Fifth sorted element should be 9.99");

    deleteDynamicArray(dynamicArray);
    printf("PASS: double operations\n");
}
void test_empty_dynamicArray_operations() {
    printf("--- Testing operations on empty dynamicArrays ---\n");

    const FieldInfo *stringFieldInfo = getStringFieldInfo();
    DynamicArray *emptyDynamicArray = createEmptyDynamicArray(stringFieldInfo);

    assert(get(emptyDynamicArray, 0) == NULL && "get on empty dynamicArray should return NULL");
    assert(get(emptyDynamicArray, -1) == NULL && "get with negative index on empty dynamicArray should return NULL");

    map(emptyDynamicArray, toUpper);
    where(emptyDynamicArray, startsWithT);

    sort(emptyDynamicArray);

    deleteDynamicArray(emptyDynamicArray);
    printf("PASS: empty dynamicArray operations\n");
}
void test_index_boundary_conditions() {
    printf("--- Testing index boundary conditions ---\n");

    const FieldInfo *stringFieldInfo = getStringFieldInfo();
    DynamicArray *dynamicArray = createEmptyDynamicArray(stringFieldInfo);

    char str1[] = "First";
    char str2[] = "Second";

    add(dynamicArray, str1);
    add(dynamicArray, str2);

    assert(get(dynamicArray, 0) != NULL && "Valid index 0 should return non-NULL");
    assert(get(dynamicArray, 1) != NULL && "Valid index 1 should return non-NULL");

    assert(get(dynamicArray, -1) == NULL && "Negative index should return NULL");
    assert(get(dynamicArray, 2) == NULL && "Index equal to size should return NULL");
    assert(get(dynamicArray, 100) == NULL && "Large out-of-bounds index should return NULL");

    deleteDynamicArray(dynamicArray);
    printf("PASS: index boundary conditions\n");
}
void test_concat_scenarios() {
    printf("--- Testing various concatenation scenarios ---\n");

    const FieldInfo *stringFieldInfo = getStringFieldInfo();

    DynamicArray *dynamicArray1 = createEmptyDynamicArray(stringFieldInfo);
    DynamicArray *dynamicArray2 = createEmptyDynamicArray(stringFieldInfo);

    char str1[] = "A";
    char str2[] = "B";
    char str3[] = "C";
    char str4[] = "D";

    add(dynamicArray1, str1);
    add(dynamicArray1, str2);

    add(dynamicArray2, str3);
    add(dynamicArray2, str4);

    DynamicArray *concatenated = concat(dynamicArray1, dynamicArray2);
    assert(concatenated != NULL && "Concatenation of two non-empty dynamicArrays should succeed");
    assert(concatenated->size == 4 && "Concatenated dynamicArray should have size 4");
    assert(strcmp((char*)get(concatenated, 0), "A") == 0 && "First element should be 'A'");
    assert(strcmp((char*)get(concatenated, 1), "B") == 0 && "Second element should be 'B'");
    assert(strcmp((char*)get(concatenated, 2), "C") == 0 && "Third element should be 'C'");
    assert(strcmp((char*)get(concatenated, 3), "D") == 0 && "Fourth element should be 'D'");

    deleteDynamicArray(concatenated);

    DynamicArray *emptyDynamicArray = createEmptyDynamicArray(stringFieldInfo);
    DynamicArray *concatWithEmpty = concat(dynamicArray1, emptyDynamicArray);
    assert(concatWithEmpty != NULL && "Concatenation with empty dynamicArray should succeed");
    assert(concatWithEmpty->size == 2 && "Result should have same size as non-empty dynamicArray");
    assert(strcmp((char*)get(concatWithEmpty, 0), "A") == 0 && "First element should be 'A'");
    assert(strcmp((char*)get(concatWithEmpty, 1), "B") == 0 && "Second element should be 'B'");

    deleteDynamicArray(concatWithEmpty);

    DynamicArray *emptyWithConcat = concat(emptyDynamicArray, dynamicArray1);
    assert(emptyWithConcat != NULL && "Concatenation of empty with non-empty should succeed");
    assert(emptyWithConcat->size == 2 && "Result should have same size as non-empty dynamicArray");
    assert(strcmp((char*)get(emptyWithConcat, 0), "A") == 0 && "First element should be 'A'");
    assert(strcmp((char*)get(emptyWithConcat, 1), "B") == 0 && "Second element should be 'B'");

    deleteDynamicArray(emptyWithConcat);
    deleteDynamicArray(dynamicArray1);
    deleteDynamicArray(dynamicArray2);
    deleteDynamicArray(emptyDynamicArray);

    printf("PASS: concatenation scenarios\n");
}
void test_string_operations() {
    printf("--- Testing string operations with special characters ---\n");

    const FieldInfo *stringFieldInfo = getStringFieldInfo();
    DynamicArray *dynamicArray = createEmptyDynamicArray(stringFieldInfo);

    char str1[] = "";
    char str2[] = " ";
    char str3[] = "Special!@#$%^&*()";
    char str4[] = "\n\t\r";

    add(dynamicArray, str1);
    add(dynamicArray, str2);
    add(dynamicArray, str3);
    add(dynamicArray, str4);

    assert(dynamicArray->size == 4 && "Size should be 4 after adding special strings");
    assert(strcmp((char*)get(dynamicArray, 0), "") == 0 && "First element should be empty string");
    assert(strcmp((char*)get(dynamicArray, 1), " ") == 0 && "Second element should be space");
    assert(strcmp((char*)get(dynamicArray, 2), "Special!@#$%^&*()") == 0 && "Third element should be special chars");
    assert(strcmp((char*)get(dynamicArray, 3), "\n\t\r") == 0 && "Fourth element should be escape chars");

    deleteDynamicArray(dynamicArray);
    printf("PASS: string operations with special characters\n");
}
int main() {
    printf("Running StringDynamicArray module tests...\n");

    test_createDynamicArray_with_data_capacity();
    test_createEmptyDynamicArray();
    test_add_and_get();
    test_get_out_of_bounds();
    test_map();
    test_where();
    test_concat();
    test_sort();
    test_deleteDynamicArray();
    test_edge_cases();
    test_data_capacity_extension();
    test_double_operations();
    test_empty_dynamicArray_operations();
    test_index_boundary_conditions();
    test_concat_scenarios();
    test_string_operations();

    printf("All tests passed!\n");

    return 0;
}
