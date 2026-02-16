//
// Created by Vasya on 15.02.2026.
//
#include "headers/FieldInfo.h"
#include "headers/List.h"

// Test helper function
void assertTrue(int condition, const char* testName) {
    if (condition) {
        printf("PASS: %s\n", testName);
    } else {
        printf("FAIL: %s\n", testName);
    }
}

void assertFalse(int condition, const char* testName) {
    assertTrue(!condition, testName);
}

void testCreateListWithCapacity() {
    printf("\n--- Testing createList ---\n");

    const FieldInfo *doubleFieldInfo = getDoubleFieldInfo();
    List *list = createList(5, doubleFieldInfo);

    assertTrue(list != NULL, "createList should return non-null pointer");
    assertTrue(list->capacity == 5, "createList should set correct capacity");
    assertTrue(list->size == 0, "createList should initialize size to 0");
    assertTrue(list->field_info == doubleFieldInfo, "createList should set correct field info");

    // Clean up
    for (int i = 0; i < list->size; i++) {
        list->field_info->deallocate(list->data[i]);
    }
    free(list->data);
    free(list);
}

void testCreateEmptyList() {
    printf("\n--- Testing createEmptyList ---\n");

    const FieldInfo *doubleFieldInfo = getDoubleFieldInfo();
    List *list = createEmptyList(doubleFieldInfo);

    assertTrue(list != NULL, "createEmptyList should return non-null pointer");
    assertTrue(list->capacity == CAPACITY, "createEmptyList should set capacity to CAPACITY constant");
    assertTrue(list->size == 0, "createEmptyList should initialize size to 0");
    assertTrue(list->field_info == doubleFieldInfo, "createEmptyList should set correct field info");

    // Clean up
    for (int i = 0; i < list->size; i++) {
        list->field_info->deallocate(list->data[i]);
    }
    free(list->data);
    free(list);
}

void testAddAndGet() {
    printf("\n--- Testing add and get with double ---\n");

    const FieldInfo *doubleFieldInfo = getDoubleFieldInfo();
    List *list = createEmptyList(doubleFieldInfo);

    double val1 = 3.14;
    double val2 = 2.71;
    double val3 = 1.41;

    add(list, &val1);
    add(list, &val2);
    add(list, &val3);

    assertTrue(list->size == 3, "Size should be 3 after adding 3 elements");
    assertTrue(*((double*)get(list, 0)) == 3.14, "First element should be 3.14");
    assertTrue(*((double*)get(list, 1)) == 2.71, "Second element should be 2.71");
    assertTrue(*((double*)get(list, 2)) == 1.41, "Third element should be 1.41");

    // Test boundary conditions
    assertTrue(get(list, -1) == NULL, "get should return NULL for negative index");
    assertTrue(get(list, 3) == NULL, "get should return NULL for out of bounds index");

    // Clean up
    for (int i = 0; i < list->size; i++) {
        list->field_info->deallocate(list->data[i]);
    }
    free(list->data);
    free(list);
}

void testAddAndGetStrings() {
    printf("\n--- Testing add and get with char* ---\n");

    const FieldInfo *stringFieldInfo = getStringFieldInfo();
    List *list = createEmptyList(stringFieldInfo);

    char str1[] = "Hello";
    char str2[] = "World";
    char str3[] = "Test";

    add(list, str1);
    add(list, str2);
    add(list, str3);

    assertTrue(list->size == 3, "Size should be 3 after adding 3 string elements");

    // Compare the values stored in the list
    assertTrue(strcmp((char*)get(list, 0), "Hello") == 0, "First string should be 'Hello'");
    assertTrue(strcmp((char*)get(list, 1), "World") == 0, "Second string should be 'World'");
    assertTrue(strcmp((char*)get(list, 2), "Test") == 0, "Third string should be 'Test'");

    // Clean up
    for (int i = 0; i < list->size; i++) {
        list->field_info->deallocate(list->data[i]);
    }
    free(list->data);
    free(list);
}

void testSet() {
    printf("\n--- Testing set with double ---\n");

    const FieldInfo *doubleFieldInfo = getDoubleFieldInfo();
    List *list = createEmptyList(doubleFieldInfo);

    double val1 = 10.0;
    double val2 = 20.0;
    double newVal = 99.99;

    add(list, &val1);
    add(list, &val2);

    assertTrue(*((double*)get(list, 1)) == 20.0, "Before set: Second element should be 20.0");

    set(list, 1, &newVal);

    assertTrue(*((double*)get(list, 1)) == 99.99, "After set: Second element should be 99.99");
    assertTrue(*((double*)get(list, 0)) == 10.0, "After set: First element should remain 10.0");

    // Test boundary conditions for set
    double outOfBoundsVal = 5.5;
    set(list, -1, &outOfBoundsVal);  // Should not modify anything
    assertTrue(list->size == 2, "set with negative index should not change size");

    set(list, 5, &outOfBoundsVal);  // Should not modify anything
    assertTrue(list->size == 2, "set with out of bounds index should not change size");

    // Clean up
    for (int i = 0; i < list->size; i++) {
        list->field_info->deallocate(list->data[i]);
    }
    free(list->data);
    free(list);
}

void testSetStrings() {
    printf("\n--- Testing set with char* ---\n");

    const FieldInfo *stringFieldInfo = getStringFieldInfo();
    List *list = createEmptyList(stringFieldInfo);

    char str1[] = "Original";
    char str2[] = "Second";
    char newStr[] = "Modified";

    add(list, str1);
    add(list, str2);

    assertTrue(strcmp((char*)get(list, 1), "Second") == 0, "Before set: Second string should be 'Second'");

    set(list, 1, newStr);

    assertTrue(strcmp((char*)get(list, 1), "Modified") == 0, "After set: Second string should be 'Modified'");
    assertTrue(strcmp((char*)get(list, 0), "Original") == 0, "After set: First string should remain 'Original'");

    // Clean up
    for (int i = 0; i < list->size; i++) {
        list->field_info->deallocate(list->data[i]);
    }
    free(list->data);
    free(list);
}
void * multiplyByTwo(void *element){
    double val = *((double*)element);
    double *result = (double*)malloc(sizeof(double));
    *result = val * 2;
    return result;
}
void testMap() {
    printf("\n--- Testing map with double ---\n");

    const FieldInfo *doubleFieldInfo = getDoubleFieldInfo();
    List *list = createEmptyList(doubleFieldInfo);

    double val1 = 1.0;
    double val2 = 2.0;
    double val3 = 3.0;

    add(list, &val1);
    add(list, &val2);
    add(list, &val3);

    map(list, multiplyByTwo);

    assertTrue(*((double*)get(list, 0)) == 2.0, "After map: First element should be 2.0");
    assertTrue(*((double*)get(list, 1)) == 4.0, "After map: Second element should be 4.0");
    assertTrue(*((double*)get(list, 2)) == 6.0, "After map: Third element should be 6.0");

    // Clean up
    for (int i = 0; i < list->size; i++) {
        list->field_info->deallocate(list->data[i]);
    }
    free(list->data);
    free(list);
}

boolean greaterThanOneHalf(const void *element) {
    double val = *((double*)element);
    return val > 1.5 ? TRUE : FALSE;
}

void testWhere() {
    printf("\n--- Testing where with double ---\n");

    const FieldInfo *doubleFieldInfo = getDoubleFieldInfo();
    List *list = createEmptyList(doubleFieldInfo);

    // Define a filter function to keep only values greater than 1.5


    double val1 = 1.0;
    double val2 = 2.0;
    double val3 = 0.5;
    double val4 = 3.0;

    add(list, &val1);
    add(list, &val2);
    add(list, &val3);
    add(list, &val4);

    assertTrue(list->size == 4, "Before where: Size should be 4");

    where(list, greaterThanOneHalf);

    assertTrue(list->size == 2, "After where: Size should be 2 (only values > 1.5)");
    assertTrue(*((double*)get(list, 0)) == 2.0, "After where: First element should be 2.0");
    assertTrue(*((double*)get(list, 1)) == 3.0, "After where: Second element should be 3.0");

    // Clean up
    for (int i = 0; i < list->size; i++) {
        list->field_info->deallocate(list->data[i]);
    }
    free(list->data);
    free(list);
}

void testConcat() {
    printf("\n--- Testing concat ---\n");

    const FieldInfo *doubleFieldInfo = getDoubleFieldInfo();
    List *list1 = createEmptyList(doubleFieldInfo);
    List *list2 = createEmptyList(doubleFieldInfo);

    double val1 = 1.0;
    double val2 = 2.0;
    double val3 = 3.0;
    double val4 = 4.0;

    add(list1, &val1);
    add(list1, &val2);

    add(list2, &val3);
    add(list2, &val4);

    List *concatenated = concat(list1, list2);

    assertTrue(concatenated != NULL, "concat should return non-null pointer");
    assertTrue(concatenated->size == 4, "concatenated list should have size 4");
    assertTrue(*((double*)get(concatenated, 0)) == 1.0, "First element of concatenated list should be 1.0");
    assertTrue(*((double*)get(concatenated, 1)) == 2.0, "Second element of concatenated list should be 2.0");
    assertTrue(*((double*)get(concatenated, 2)) == 3.0, "Third element of concatenated list should be 3.0");
    assertTrue(*((double*)get(concatenated, 3)) == 4.0, "Fourth element of concatenated list should be 4.0");

    // Test concatenating with different field info types (should return NULL)
    const FieldInfo *stringFieldInfo = getStringFieldInfo();
    List *stringList = createEmptyList(stringFieldInfo);
    char testStr[] = "test";
    add(stringList, testStr);

    List *invalidConcat = concat(list1, stringList);
    assertTrue(invalidConcat == NULL, "concat should return NULL when field infos differ");

    // Clean up
    for (int i = 0; i < list1->size; i++) {
        list1->field_info->deallocate(list1->data[i]);
    }
    free(list1->data);
    free(list1);

    for (int i = 0; i < list2->size; i++) {
        list2->field_info->deallocate(list2->data[i]);
    }
    free(list2->data);
    free(list2);

    for (int i = 0; i < concatenated->size; i++) {
        concatenated->field_info->deallocate(concatenated->data[i]);
    }
    free(concatenated->data);
    free(concatenated);

    for (int i = 0; i < stringList->size; i++) {
        stringList->field_info->deallocate(stringList->data[i]);
    }
    free(stringList->data);
    free(stringList);
}

void testSort() {
    printf("\n--- Testing sort with double ---\n");

    const FieldInfo *doubleFieldInfo = getDoubleFieldInfo();
    List *list = createEmptyList(doubleFieldInfo);

    // Add elements in descending order
    double val1 = 5.0;
    double val2 = 3.0;
    double val3 = 8.0;
    double val4 = 1.0;
    double val5 = 9.0;

    add(list, &val1);
    add(list, &val2);
    add(list, &val3);
    add(list, &val4);
    add(list, &val5);

    assertTrue(list->size == 5, "Before sort: Size should be 5");
    assertTrue(*((double*)get(list, 0)) == 5.0, "Before sort: First element should be 5.0");

    sort(list);

    // After sorting in ascending order based on the comparison function
    // Note: The comparison function returns TRUE if a >= b, so sort will arrange in descending order
    assertTrue(*((double*)get(list, 0)) == 1.0, "After sort: First element should be 9.0");
    assertTrue(*((double*)get(list, 1)) == 3.0, "After sort: Second element should be 8.0");
    assertTrue(*((double*)get(list, 2)) == 5.0, "After sort: Third element should be 5.0");
    assertTrue(*((double*)get(list, 3)) == 8.0, "After sort: Fourth element should be 3.0");
    assertTrue(*((double*)get(list, 4)) == 9.0, "After sort: Fifth element should be 1.0");

    // Test sorting empty list
    List *emptyList = createEmptyList(doubleFieldInfo);
    sort(emptyList);  // Should not crash
    assertTrue(emptyList->size == 0, "Sorting empty list should keep size 0");

    // Clean up
    for (int i = 0; i < list->size; i++) {
        list->field_info->deallocate(list->data[i]);
    }
    free(list->data);
    free(list);

    for (int i = 0; i < emptyList->size; i++) {
        emptyList->field_info->deallocate(emptyList->data[i]);
    }
    free(emptyList->data);
    free(emptyList);
}

void testEdgeCases() {
    printf("\n--- Testing edge cases ---\n");

    const FieldInfo *doubleFieldInfo = getDoubleFieldInfo();

    // Test operations on NULL list
    List *nullList = NULL;
    assertTrue(get(nullList, 0) == NULL, "get on NULL list should return NULL");

    double dummyVal = 1.0;
    set(nullList, 0, &dummyVal);  // Should not crash

    add(nullList, &dummyVal);  // Should not crash

    // Create a valid list for further tests
    List *list = createEmptyList(doubleFieldInfo);

    // Test adding NULL element
    add(list, NULL);  // Should not add anything
    assertTrue(list->size == 0, "Adding NULL element should not increase size");

    // Test field_info NULL check in createEmptyList
    List *listFromNullField = createEmptyList(NULL);
    assertTrue(listFromNullField == NULL, "createEmptyList with NULL field_info should return NULL");

    // Clean up
    for (int i = 0; i < list->size; i++) {
        list->field_info->deallocate(list->data[i]);
    }
    free(list->data);
    free(list);
}

int main() {
    printf("Running List module tests...\n");

    testCreateListWithCapacity();
    testCreateEmptyList();
    testAddAndGet();
    testAddAndGetStrings();
    testSet();
    testSetStrings();
    testMap();
    testWhere();
    testConcat();
    testSort();
    testEdgeCases();

    printf("\nAll tests completed!\n");

    return 0;
}
