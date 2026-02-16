//
// Created by Vasya on 15.02.2026.
//
#include "headers/FieldInfo.h"
#include "headers/List.h"
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

void test_createList_with_capacity() {
    printf("--- Testing createList with capacity ---\n");

    const FieldInfo *stringFieldInfo = getStringFieldInfo();
    List *list = createList(3, stringFieldInfo);

    assert(list != NULL && "createList should return non-null pointer");
    assert(list->capacity == 3 && "createList should set correct capacity");
    assert(list->size == 0 && "createList should initialize size to 0");
    assert(list->field_info == stringFieldInfo && "createList should set correct field info");

    deleteList(list);
    printf("PASS: createList with capacity\n");
}

void test_createEmptyList() {
    printf("--- Testing createEmptyList ---\n");

    const FieldInfo *stringFieldInfo = getStringFieldInfo();
    List *list = createEmptyList(stringFieldInfo);

    assert(list != NULL && "createEmptyList should return non-null pointer");
    assert(list->capacity == CAPACITY && "createEmptyList should set capacity to CAPACITY constant");
    assert(list->size == 0 && "createEmptyList should initialize size to 0");
    assert(list->field_info == stringFieldInfo && "createEmptyList should set correct field info");

    deleteList(list);
    printf("PASS: createEmptyList\n");
}

void test_add_and_get() {
    printf("--- Testing add and get ---\n");

    const FieldInfo *stringFieldInfo = getStringFieldInfo();
    List *list = createEmptyList(stringFieldInfo);

    char str1[] = "Hello";
    char str2[] = "World";
    char str3[] = "Test";

    add(list, str1);
    add(list, str2);
    add(list, str3);

    assert(list->size == 3 && "Size should be 3 after adding 3 elements");
    assert(strcmp((char*)get(list, 0), "Hello") == 0 && "First element should be 'Hello'");
    assert(strcmp((char*)get(list, 1), "World") == 0 && "Second element should be 'World'");
    assert(strcmp((char*)get(list, 2), "Test") == 0 && "Third element should be 'Test'");

    deleteList(list);
    printf("PASS: add and get\n");
}

void test_get_out_of_bounds() {
    printf("--- Testing get with out of bounds index ---\n");

    const FieldInfo *stringFieldInfo = getStringFieldInfo();
    List *list = createEmptyList(stringFieldInfo);

    assert(get(list, 0) == NULL && "get should return NULL for empty list");
    assert(get(list, -1) == NULL && "get should return NULL for negative index");
    assert(get(list, 1) == NULL && "get should return NULL for out of bounds index");

    deleteList(list);
    printf("PASS: get out of bounds\n");
}

void test_set() {
    printf("--- Testing set ---\n");

    const FieldInfo *stringFieldInfo = getStringFieldInfo();
    List *list = createEmptyList(stringFieldInfo);

    char str1[] = "Initial";
    char str2[] = "Second";
    char newStr[] = "Updated";

    add(list, str1);
    add(list, str2);

    assert(strcmp((char*)get(list, 1), "Second") == 0 && "Before set: Second element should be 'Second'");

    set(list, 1, newStr);

    assert(strcmp((char*)get(list, 1), "Updated") == 0 && "After set: Second element should be 'Updated'");
    assert(strcmp((char*)get(list, 0), "Initial") == 0 && "After set: First element should remain 'Initial'");

    deleteList(list);
    printf("PASS: set\n");
}

void test_set_out_of_bounds() {
    printf("--- Testing set with out of bounds index ---\n");

    const FieldInfo *stringFieldInfo = getStringFieldInfo();
    List *list = createEmptyList(stringFieldInfo);

    char str1[] = "Initial";
    char newStr[] = "NewValue";

    add(list, str1);

    set(list, -1, newStr);  // Should not modify anything
    assert(list->size == 1 && "set with negative index should not change size");

    set(list, 5, newStr);  // Should not modify anything
    assert(list->size == 1 && "set with out of bounds index should not change size");

    deleteList(list);
    printf("PASS: set out of bounds\n");
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
    List *list = createEmptyList(stringFieldInfo);

    char str1[] = "hello";
    char str2[] = "world";

    add(list, str1);
    add(list, str2);

    map(list, toUpper);

    assert(strcmp((char*)get(list, 0), "HELLO") == 0 && "After map: First element should be 'HELLO'");
    assert(strcmp((char*)get(list, 1), "WORLD") == 0 && "After map: Second element should be 'WORLD'");

    deleteList(list);
    printf("PASS: map\n");
}

boolean startsWithT(const void *element) {
    const char *str = (const char*)element;
    return str[0] == 'T' ? TRUE : FALSE;
}

void test_where() {
    printf("--- Testing where ---\n");

    const FieldInfo *stringFieldInfo = getStringFieldInfo();
    List *list = createEmptyList(stringFieldInfo);

    char str1[] = "Test";
    char str2[] = "Hello";
    char str3[] = "Today";
    char str4[] = "World";

    add(list, str1);
    add(list, str2);
    add(list, str3);
    add(list, str4);

    assert(list->size == 4 && "Before where: Size should be 4");

    where(list, startsWithT);

    assert(list->size == 2 && "After where: Size should be 2 (only strings starting with 'T')");
    assert(strcmp((char*)get(list, 0), "Test") == 0 && "After where: First element should be 'Test'");
    assert(strcmp((char*)get(list, 1), "Today") == 0 && "After where: Second element should be 'Today'");

    deleteList(list);
    printf("PASS: where\n");
}

void test_concat() {
    printf("--- Testing concat ---\n");

    const FieldInfo *stringFieldInfo = getStringFieldInfo();
    List *list1 = createEmptyList(stringFieldInfo);
    List *list2 = createEmptyList(stringFieldInfo);

    char str1[] = "Hello";
    char str2[] = "World";
    char str3[] = "Test";
    char str4[] = "Concat";

    add(list1, str1);
    add(list1, str2);

    add(list2, str3);
    add(list2, str4);

    List *concatenated = concat(list1, list2);

    assert(concatenated != NULL && "concat should return non-null pointer");
    assert(concatenated->size == 4 && "concatenated list should have size 4");
    assert(strcmp((char*)get(concatenated, 0), "Hello") == 0 && "First element of concatenated list should be 'Hello'");
    assert(strcmp((char*)get(concatenated, 1), "World") == 0 && "Second element of concatenated list should be 'World'");
    assert(strcmp((char*)get(concatenated, 2), "Test") == 0 && "Third element of concatenated list should be 'Test'");
    assert(strcmp((char*)get(concatenated, 3), "Concat") == 0 && "Fourth element of concatenated list should be 'Concat'");

    deleteList(list1);
    deleteList(list2);
    deleteList(concatenated);
    printf("PASS: concat\n");
}

void test_sort() {
    printf("--- Testing sort ---\n");

    const FieldInfo *stringFieldInfo = getStringFieldInfo();
    List *list = createEmptyList(stringFieldInfo);

    char str1[] = "zebra";
    char str2[] = "apple";
    char str3[] = "banana";

    add(list, str1);
    add(list, str2);
    add(list, str3);

    assert(list->size == 3 && "Before sort: Size should be 3");

    sort(list);

    assert(strcmp((char*)get(list, 0), "apple") == 0 && "After sort: First element should be 'apple'");
    assert(strcmp((char*)get(list, 1), "banana") == 0 && "After sort: Second element should be 'banana'");
    assert(strcmp((char*)get(list, 2), "zebra") == 0 && "After sort: Third element should be 'zebra'");

    deleteList(list);
    printf("PASS: sort\n");
}

void test_deleteList() {
    printf("--- Testing deleteList ---\n");

    const FieldInfo *stringFieldInfo = getStringFieldInfo();
    List *list = createEmptyList(stringFieldInfo);

    char str1[] = "ToDelete";
    char str2[] = "AlsoToDelete";

    add(list, str1);
    add(list, str2);

    assert(list->size == 2 && "Size should be 2 before deletion");

    deleteList(list);

    // No direct way to assert memory deallocation, but we ensure no crash occurs
    printf("PASS: deleteList\n");
}

void test_edge_cases() {
    printf("--- Testing edge cases ---\n");

    const FieldInfo *stringFieldInfo = getStringFieldInfo();

    // Test operations on NULL list
    List *nullList = NULL;
    assert(get(nullList, 0) == NULL && "get on NULL list should return NULL");

    char dummyStr[] = "dummy";
    set(nullList, 0, dummyStr);  // Should not crash
    add(nullList, dummyStr);     // Should not crash

    // Create a valid list for further tests
    List *list = createEmptyList(stringFieldInfo);

    // Test adding NULL element
    add(list, NULL);  // Should not add anything
    assert(list->size == 0 && "Adding NULL element should not increase size");

    // Test field_info NULL check in createEmptyList
    List *listFromNullField = createEmptyList(NULL);
    assert(listFromNullField == NULL && "createEmptyList with NULL field_info should return NULL");

    deleteList(list);
    printf("PASS: edge cases\n");
}

void test_capacity_extension() {
    printf("--- Testing capacity extension ---\n");

    const FieldInfo *stringFieldInfo = getStringFieldInfo();
    List *list = createList(2, stringFieldInfo);  // Start with small capacity

    // Add elements beyond initial capacity to trigger expansion
    char str1[] = "First";
    char str2[] = "Second";
    char str3[] = "Third";  // This should trigger capacity expansion
    char str4[] = "Fourth";
    char str5[] = "Fifth";

    assert(list->capacity == 2 && "Initial capacity should be 2");
    assert(list->size == 0 && "Initial size should be 0");

    add(list, str1);
    assert(list->capacity == 2 && "Capacity should remain 2 after adding first element");
    assert(list->size == 1 && "Size should be 1 after adding first element");

    add(list, str2);
    assert(list->capacity == 2 && "Capacity should remain 2 after adding second element");
    assert(list->size == 2 && "Size should be 2 after adding second element");

    add(list, str3);
    assert(list->capacity == 4 && "Capacity should double to 4 after exceeding initial capacity");
    assert(list->size == 3 && "Size should be 3 after adding third element");

    add(list, str4);
    assert(list->capacity == 4 && "Capacity should remain 4 after adding fourth element");
    assert(list->size == 4 && "Size should be 4 after adding fourth element");

    add(list, str5);
    assert(list->capacity == 8 && "Capacity should double to 8 after exceeding extended capacity");
    assert(list->size == 5 && "Size should be 5 after adding fifth element");

    // Verify all elements are accessible
    assert(strcmp((char*)get(list, 0), "First") == 0 && "First element should be 'First'");
    assert(strcmp((char*)get(list, 1), "Second") == 0 && "Second element should be 'Second'");
    assert(strcmp((char*)get(list, 2), "Third") == 0 && "Third element should be 'Third'");
    assert(strcmp((char*)get(list, 3), "Fourth") == 0 && "Fourth element should be 'Fourth'");
    assert(strcmp((char*)get(list, 4), "Fifth") == 0 && "Fifth element should be 'Fifth'");

    deleteList(list);
    printf("PASS: capacity extension\n");
}

void test_double_operations() {
    printf("--- Testing operations with double values ---\n");

    const FieldInfo *doubleFieldInfo = getDoubleFieldInfo();
    List *list = createEmptyList(doubleFieldInfo);

    double val1 = 3.14;
    double val2 = 2.71;
    double val3 = 1.41;

    add(list, &val1);
    add(list, &val2);
    add(list, &val3);

    assert(list->size == 3 && "Size should be 3 after adding 3 doubles");

    // Check values using proper comparison for doubles
    double *retrieved_val1 = (double*)get(list, 0);
    double *retrieved_val2 = (double*)get(list, 1);
    double *retrieved_val3 = (double*)get(list, 2);

    assert(fabs(*retrieved_val1 - 3.14) < 1e-9 && "First element should be 3.14");
    assert(fabs(*retrieved_val2 - 2.71) < 1e-9 && "Second element should be 2.71");
    assert(fabs(*retrieved_val3 - 1.41) < 1e-9 && "Third element should be 1.41");

    // Test set operation with doubles
    double newVal = 9.99;
    set(list, 1, &newVal);
    double *updatedVal = (double*)get(list, 1);
    assert(fabs(*updatedVal - 9.99) < 1e-9 && "Second element should be updated to 9.99");

    // Test sort with doubles
    double val4 = 0.5;
    double val5 = 5.0;
    add(list, &val4);
    add(list, &val5);

    sort(list);

    // After sorting: 0.5, 1.41, 3.14, 5.0, 9.99
    double *sortedVal0 = (double*)get(list, 0);
    double *sortedVal1 = (double*)get(list, 1);
    double *sortedVal2 = (double*)get(list, 2);
    double *sortedVal3 = (double*)get(list, 3);
    double *sortedVal4 = (double*)get(list, 4);

    assert(fabs(*sortedVal0 - 0.5) < 1e-9 && "First sorted element should be 0.5");
    assert(fabs(*sortedVal1 - 1.41) < 1e-9 && "Second sorted element should be 1.41");
    assert(fabs(*sortedVal2 - 3.14) < 1e-9 && "Third sorted element should be 3.14");
    assert(fabs(*sortedVal3 - 5.0) < 1e-9 && "Fourth sorted element should be 5.0");
    assert(fabs(*sortedVal4 - 9.99) < 1e-9 && "Fifth sorted element should be 9.99");

    deleteList(list);
    printf("PASS: double operations\n");
}

void test_empty_list_operations() {
    printf("--- Testing operations on empty lists ---\n");

    const FieldInfo *stringFieldInfo = getStringFieldInfo();
    List *emptyList = createEmptyList(stringFieldInfo);

    // Test get on empty list
    assert(get(emptyList, 0) == NULL && "get on empty list should return NULL");
    assert(get(emptyList, -1) == NULL && "get with negative index on empty list should return NULL");

    // Test set on empty list (should do nothing)
    char newValue[] = "ShouldNotSet";
    set(emptyList, 0, newValue);
    assert(emptyList->size == 0 && "set on empty list should not change size");

    // Test operations on empty list shouldn't crash
    map(emptyList, toUpper);
    where(emptyList, startsWithT);
    
    // Test sort on empty list
    sort(emptyList);

    deleteList(emptyList);
    printf("PASS: empty list operations\n");
}

void test_index_boundary_conditions() {
    printf("--- Testing index boundary conditions ---\n");

    const FieldInfo *stringFieldInfo = getStringFieldInfo();
    List *list = createEmptyList(stringFieldInfo);

    char str1[] = "First";
    char str2[] = "Second";

    add(list, str1);
    add(list, str2);

    // Test valid indices
    assert(get(list, 0) != NULL && "Valid index 0 should return non-NULL");
    assert(get(list, 1) != NULL && "Valid index 1 should return non-NULL");

    // Test boundary invalid indices
    assert(get(list, -1) == NULL && "Negative index should return NULL");
    assert(get(list, 2) == NULL && "Index equal to size should return NULL");
    assert(get(list, 100) == NULL && "Large out-of-bounds index should return NULL");

    // Test set with boundary conditions
    char newValue[] = "New";
    set(list, 2, newValue);  // Out of bounds, should not modify
    assert(list->size == 2 && "set with out-of-bounds index should not change size");

    set(list, -1, newValue);  // Negative index, should not modify
    assert(list->size == 2 && "set with negative index should not change size");

    deleteList(list);
    printf("PASS: index boundary conditions\n");
}

void test_concat_scenarios() {
    printf("--- Testing various concatenation scenarios ---\n");

    const FieldInfo *stringFieldInfo = getStringFieldInfo();

    // Test concatenating two non-empty lists
    List *list1 = createEmptyList(stringFieldInfo);
    List *list2 = createEmptyList(stringFieldInfo);

    char str1[] = "A";
    char str2[] = "B";
    char str3[] = "C";
    char str4[] = "D";

    add(list1, str1);
    add(list1, str2);

    add(list2, str3);
    add(list2, str4);

    List *concatenated = concat(list1, list2);
    assert(concatenated != NULL && "Concatenation of two non-empty lists should succeed");
    assert(concatenated->size == 4 && "Concatenated list should have size 4");
    assert(strcmp((char*)get(concatenated, 0), "A") == 0 && "First element should be 'A'");
    assert(strcmp((char*)get(concatenated, 1), "B") == 0 && "Second element should be 'B'");
    assert(strcmp((char*)get(concatenated, 2), "C") == 0 && "Third element should be 'C'");
    assert(strcmp((char*)get(concatenated, 3), "D") == 0 && "Fourth element should be 'D'");

    deleteList(concatenated);

    // Test concatenating with empty list
    List *emptyList = createEmptyList(stringFieldInfo);
    List *concatWithEmpty = concat(list1, emptyList);
    assert(concatWithEmpty != NULL && "Concatenation with empty list should succeed");
    assert(concatWithEmpty->size == 2 && "Result should have same size as non-empty list");
    assert(strcmp((char*)get(concatWithEmpty, 0), "A") == 0 && "First element should be 'A'");
    assert(strcmp((char*)get(concatWithEmpty, 1), "B") == 0 && "Second element should be 'B'");

    deleteList(concatWithEmpty);

    // Test concatenating empty with non-empty
    List *emptyWithConcat = concat(emptyList, list1);
    assert(emptyWithConcat != NULL && "Concatenation of empty with non-empty should succeed");
    assert(emptyWithConcat->size == 2 && "Result should have same size as non-empty list");
    assert(strcmp((char*)get(emptyWithConcat, 0), "A") == 0 && "First element should be 'A'");
    assert(strcmp((char*)get(emptyWithConcat, 1), "B") == 0 && "Second element should be 'B'");

    deleteList(emptyWithConcat);
    deleteList(list1);
    deleteList(list2);
    deleteList(emptyList);

    printf("PASS: concatenation scenarios\n");
}

void test_string_operations() {
    printf("--- Testing string operations with special characters ---\n");

    const FieldInfo *stringFieldInfo = getStringFieldInfo();
    List *list = createEmptyList(stringFieldInfo);

    char str1[] = "";
    char str2[] = " ";
    char str3[] = "Special!@#$%^&*()";
    char str4[] = "\n\t\r";

    add(list, str1);
    add(list, str2);
    add(list, str3);
    add(list, str4);

    assert(list->size == 4 && "Size should be 4 after adding special strings");
    assert(strcmp((char*)get(list, 0), "") == 0 && "First element should be empty string");
    assert(strcmp((char*)get(list, 1), " ") == 0 && "Second element should be space");
    assert(strcmp((char*)get(list, 2), "Special!@#$%^&*()") == 0 && "Third element should be special chars");
    assert(strcmp((char*)get(list, 3), "\n\t\r") == 0 && "Fourth element should be escape chars");

    deleteList(list);
    printf("PASS: string operations with special characters\n");
}

int main() {
    printf("Running StringList module tests...\n");

    test_createList_with_capacity();
    test_createEmptyList();
    test_add_and_get();
    test_get_out_of_bounds();
    test_set();
    test_set_out_of_bounds();
    test_map();
    test_where();
    test_concat();
    test_sort();
    test_deleteList();
    test_edge_cases();
    test_capacity_extension();
    test_double_operations();
    test_empty_list_operations();
    test_index_boundary_conditions();
    test_concat_scenarios();
    test_string_operations();

    printf("All tests passed!\n");

    return 0;
}
