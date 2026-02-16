//
// Created by Vasya on 15.02.2026.
//
#include "headers/FieldInfo.h"
#include "headers/List.h"
#include <assert.h>
#include <ctype.h>
#include <string.h>

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
    test_string_operations();

    printf("All tests passed!\n");

    return 0;
}
