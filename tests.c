// test_dynamic_array.c
// Full test suite for DynamicArray using only assert

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include "../firstlab/headers/DynamicArray.h"
#include "../firstlab/headers/FieldInfo.h"
#include "../firstlab/headers/UsersStruct.h"

// ============================================================================
// HELPER FUNCTIONS FOR TESTS
// ============================================================================

// map() helper: multiply double by 2
// Returns malloc'd memory because map() calls free() on the result
void *multiply_by_two(void *x) {
    if (x == NULL) {
        return NULL;
    }
    double *val = (double *)x;
    double *result = (double *)malloc(sizeof(double));
    if (result == NULL) {
        return NULL;
    }
    *result = *val * 2.0;
    return result;
}

// map() helper: identity function
// Returns malloc'd copy of the input value
void *identity_function(void *x) {
    if (x == NULL) {
        return NULL;
    }
    const FieldInfo *fi = get_double_field_info();
    if (fi == NULL) {
        return NULL;
    }
    void *result = malloc(fi->size);
    if (result == NULL) {
        return NULL;
    }
    fi->assign(result, x);
    return result;
}

// where() helper: predicate that always returns true
bool always_true(const void *x) {
    (void)x;  // Suppress unused parameter warning
    return true;
}

// where() helper: predicate that always returns false
bool always_false(const void *x) {
    (void)x;  // Suppress unused parameter warning
    return false;
}

// where() helper: filter positive double values
bool is_positive(const void *x) {
    const double *val = (const double *)x;
    return *val > 0;
}

// where() helper: filter negative double values
bool is_negative(const void *x) {
    const double *val = (const double *)x;
    return *val < 0;
}

// ============================================================================
// ARRAY CREATION TESTS
// ============================================================================

// Test create_dynamic_array with valid and invalid parameters
void test_create_dynamic_array() {
    printf("Testing create_dynamic_array... ");

    const FieldInfo *fi = get_double_field_info();
    assert(fi != NULL);

    // Valid creation with capacity 10
    DynamicArray *arr = create_dynamic_array(10, fi);
    assert(arr != NULL);
    assert(arr->size == 0);
    assert(arr->data_capacity == 10);
    assert(arr->field_info == fi);
    assert(arr->data != NULL);
    destroy_dynamic_array(arr);

    // NULL FieldInfo should fail
    errno = 0;
    arr = create_dynamic_array(10, NULL);
    assert(arr == NULL);
    assert(errno == EINVAL);

    // Negative capacity should fail
    errno = 0;
    arr = create_dynamic_array(-5, fi);
    assert(arr == NULL);
    assert(errno == EINVAL);

    // Zero capacity should fail
    errno = 0;
    arr = create_dynamic_array(0, fi);
    assert(arr == NULL);
    assert(errno == EINVAL);

    printf("OK\n");
}

// Test create_empty_dynamic_array with valid and invalid parameters
void test_create_empty_dynamic_array() {
    printf("Testing create_empty_dynamic_array... ");

    const FieldInfo *fi = get_string_field_info();
    assert(fi != NULL);

    // Valid creation with default capacity
    DynamicArray *arr = create_empty_dynamic_array(fi);
    assert(arr != NULL);
    assert(arr->size == 0);
    assert(arr->data_capacity == 10);  // START_DATA_CAPACITY
    assert(arr->field_info == fi);
    assert(arr->data != NULL);
    destroy_dynamic_array(arr);

    // NULL FieldInfo should fail
    errno = 0;
    arr = create_empty_dynamic_array(NULL);
    assert(arr == NULL);
    assert(errno == EINVAL);

    printf("OK\n");
}

// ============================================================================
// ADD() TESTS
// ============================================================================

// Test add() with edge cases and normal operation
void test_add() {
    printf("Testing add... ");

    const FieldInfo *fi = get_double_field_info();
    DynamicArray *arr = create_empty_dynamic_array(fi);
    assert(arr != NULL);

    // NULL array should fail
    errno = 0;
    double val = 1.0;
    assert(add(NULL, &val) == false);
    assert(errno == EINVAL);

    // NULL element should fail
    errno = 0;
    assert(add(arr, NULL) == false);
    assert(errno == EINVAL);

    // Add first element
    double value1 = 3.14;
    assert(add(arr, &value1) == true);
    assert(arr->size == 1);

    // Verify added element
    double *retrieved = (double *)get(arr, 0);
    assert(retrieved != NULL);
    assert(*retrieved == 3.14);

    // Fill to capacity
    for (int i = 0; i < 9; i++) {
        double v = (double)i;
        assert(add(arr, &v) == true);
    }
    assert(arr->size == 10);
    assert(arr->data_capacity == 10);

    // Trigger resize by adding one more element
    double value2 = 2.71;
    assert(add(arr, &value2) == true);
    assert(arr->size == 11);
    assert(arr->data_capacity == 20);  // GROW_FACTOR = 2

    // Verify new element after resize
    retrieved = (double *)get(arr, 10);
    assert(retrieved != NULL);
    assert(*retrieved == 2.71);

    // Verify first element was not lost during resize
    retrieved = (double *)get(arr, 0);
    assert(retrieved != NULL);
    assert(*retrieved == 3.14);

    destroy_dynamic_array(arr);

    printf("OK\n");
}

// ============================================================================
// GET() TESTS
// ============================================================================

// Test get() with boundary conditions and valid access
void test_get() {
    printf("Testing get... ");

    const FieldInfo *fi = get_double_field_info();
    DynamicArray *arr = create_empty_dynamic_array(fi);
    assert(arr != NULL);

    // NULL array should fail
    errno = 0;
    assert(get(NULL, 0) == NULL);
    assert(errno == EINVAL);

    // Negative index should fail
    errno = 0;
    assert(get(arr, -1) == NULL);
    assert(errno == EINVAL);

    // Index >= size on empty array should fail
    errno = 0;
    assert(get(arr, 0) == NULL);
    assert(errno == EINVAL);

    // Add elements and verify retrieval
    double values[] = {1.41, 2.71, 3.14};
    for (int i = 0; i < 3; i++) {
        assert(add(arr, &values[i]) == true);
    }

    for (int i = 0; i < 3; i++) {
        double *retrieved = (double *)get(arr, i);
        assert(retrieved != NULL);
        assert(*retrieved == values[i]);
    }

    // Index >= size on filled array should fail
    errno = 0;
    assert(get(arr, 3) == NULL);
    assert(errno == EINVAL);

    destroy_dynamic_array(arr);

    printf("OK\n");
}

// ============================================================================
// SET() TESTS
// ============================================================================

// Test set() with boundary conditions and value replacement
void test_set() {
    printf("Testing set... ");

    const FieldInfo *fi = get_double_field_info();
    DynamicArray *arr = create_empty_dynamic_array(fi);
    assert(arr != NULL);

    // NULL array should fail
    errno = 0;
    double val = 1.0;
    assert(set(NULL, 0, &val) == false);
    assert(errno == EINVAL);

    // NULL value should fail
    errno = 0;
    assert(set(arr, 0, NULL) == false);
    assert(errno == EINVAL);

    // Index out of range on empty array should fail
    errno = 0;
    assert(set(arr, 0, &val) == false);
    assert(errno == EINVAL);

    // Valid replacement
    double value1 = 3.14;
    double value2 = 2.71;
    double value3 = 1.41;
    assert(add(arr, &value1) == true);
    assert(add(arr, &value2) == true);

    assert(set(arr, 0, &value3) == true);
    double *retrieved = (double *)get(arr, 0);
    assert(*retrieved == 1.41);

    // Verify second element was not modified
    retrieved = (double *)get(arr, 1);
    assert(*retrieved == 2.71);

    // Negative index should fail
    errno = 0;
    assert(set(arr, -1, &value1) == false);
    assert(errno == EINVAL);

    // Index >= size should fail
    errno = 0;
    assert(set(arr, 5, &value1) == false);
    assert(errno == EINVAL);

    destroy_dynamic_array(arr);

    printf("OK\n");
}

// ============================================================================
// MAP() TESTS
// ============================================================================

// Test map() with edge cases and transformation logic
void test_map() {
    printf("Testing map... ");

    const FieldInfo *fi = get_double_field_info();
    DynamicArray *arr = create_empty_dynamic_array(fi);
    assert(arr != NULL);

    // NULL array should fail
    errno = 0;
    assert(map(NULL, multiply_by_two) == NULL);
    assert(errno == EINVAL);

    // NULL function should fail
    errno = 0;
    assert(map(arr, NULL) == NULL);
    assert(errno == EINVAL);

    // Empty array should return empty result
    DynamicArray *result = map(arr, multiply_by_two);
    assert(result != NULL);
    assert(result->size == 0);
    assert(result->data_capacity == 10);
    destroy_dynamic_array(result);

    // Normal mapping: multiply all elements by 2
    double values[] = {1.0, 2.0, 3.0, 4.0, 5.0};
    for (int i = 0; i < 5; i++) {
        assert(add(arr, &values[i]) == true);
    }

    result = map(arr, multiply_by_two);
    assert(result != NULL);
    assert(result->size == 5);

    for (int i = 0; i < 5; i++) {
        double *r = (double *)get(result, i);
        assert(r != NULL);
        assert(*r == values[i] * 2.0);
    }

    // Verify original array was not modified
    for (int i = 0; i < 5; i++) {
        double *orig = (double *)get(arr, i);
        assert(*orig == values[i]);
    }

    destroy_dynamic_array(arr);
    destroy_dynamic_array(result);

    // Test identity function mapping
    arr = create_empty_dynamic_array(fi);
    double identity_values[] = {10.0, 20.0, 30.0};
    for (int i = 0; i < 3; i++) {
        assert(add(arr, &identity_values[i]) == true);
    }

    result = map(arr, identity_function);
    assert(result != NULL);
    assert(result->size == 3);

    for (int i = 0; i < 3; i++) {
        double *r = (double *)get(result, i);
        assert(r != NULL);
        assert(*r == identity_values[i]);
    }

    destroy_dynamic_array(arr);
    destroy_dynamic_array(result);

    // Test map with NULL function (already tested but ensure cleanup)
    arr = create_empty_dynamic_array(fi);
    double test_val = 5.0;
    assert(add(arr, &test_val) == true);

    result = map(arr, NULL);
    assert(result == NULL);

    destroy_dynamic_array(arr);

    printf("OK\n");
}

// ============================================================================
// WHERE() TESTS
// ============================================================================

// Test where() with filtering predicates
void test_where() {
    printf("Testing where... ");

    const FieldInfo *fi = get_double_field_info();
    DynamicArray *arr = create_empty_dynamic_array(fi);
    assert(arr != NULL);

    // NULL array should fail
    errno = 0;
    assert(where(NULL, is_positive) == NULL);
    assert(errno == EINVAL);

    // NULL function should fail
    errno = 0;
    assert(where(arr, NULL) == NULL);
    assert(errno == EINVAL);

    // Empty array should return empty result
    DynamicArray *result = where(arr, always_true);
    assert(result != NULL);
    assert(result->size == 0);
    destroy_dynamic_array(result);

    // All elements match with always_true predicate
    double values[] = {1.0, 2.0, 3.0};
    for (int i = 0; i < 3; i++) {
        assert(add(arr, &values[i]) == true);
    }

    result = where(arr, always_true);
    assert(result != NULL);
    assert(result->size == 3);
    destroy_dynamic_array(result);

    // No elements match with always_false predicate
    result = where(arr, always_false);
    assert(result != NULL);
    assert(result->size == 0);
    destroy_dynamic_array(result);

    // Filter positive numbers from mixed array
    destroy_dynamic_array(arr);
    arr = create_empty_dynamic_array(fi);

    double filter_values[] = {-1.0, 2.0, -3.0, 4.0, -5.0, 6.0};
    for (int i = 0; i < 6; i++) {
        assert(add(arr, &filter_values[i]) == true);
    }

    result = where(arr, is_positive);
    assert(result != NULL);
    assert(result->size == 3);

    double expected_positive[] = {2.0, 4.0, 6.0};
    for (int i = 0; i < 3; i++) {
        double *r = (double *)get(result, i);
        assert(r != NULL);
        assert(*r == expected_positive[i]);
    }

    destroy_dynamic_array(arr);
    destroy_dynamic_array(result);

    printf("OK\n");
}

// ============================================================================
// CONCAT() TESTS
// ============================================================================

// Test concat() with edge cases and normal concatenation
void test_concat() {
    printf("Testing concat... ");

    const FieldInfo *fi = get_double_field_info();
    DynamicArray *arr1 = create_empty_dynamic_array(fi);
    DynamicArray *arr2 = create_empty_dynamic_array(fi);
    assert(arr1 != NULL);
    assert(arr2 != NULL);

    // NULL first array should fail
    errno = 0;
    assert(concat(NULL, arr2) == NULL);
    assert(errno == EINVAL);

    // NULL second array should fail
    errno = 0;
    assert(concat(arr1, NULL) == NULL);
    assert(errno == EINVAL);

    // Different FieldInfo types should fail
    const FieldInfo *fi_string = get_string_field_info();
    DynamicArray *arr3 = create_empty_dynamic_array(fi_string);
    assert(arr3 != NULL);
    errno = 0;
    assert(concat(arr1, arr3) == NULL);
    assert(errno == EINVAL);
    destroy_dynamic_array(arr3);

    // Concatenating two empty arrays
    DynamicArray *result = concat(arr1, arr2);
    assert(result != NULL);
    assert(result->size == 0);
    destroy_dynamic_array(result);

    // Concatenate empty + non-empty (arr1 empty)
    double v2[] = {3.0, 4.0, 5.0};
    for (int i = 0; i < 3; i++) {
        assert(add(arr2, &v2[i]) == true);
    }

    result = concat(arr1, arr2);
    assert(result != NULL);
    assert(result->size == 3);
    for (int i = 0; i < 3; i++) {
        double *val = (double *)get(result, i);
        assert(*val == v2[i]);
    }
    destroy_dynamic_array(result);

    // Concatenate non-empty + empty (arr2 empty)
    destroy_dynamic_array(arr2);
    arr2 = create_empty_dynamic_array(fi);

    double v1[] = {1.0, 2.0};
    for (int i = 0; i < 2; i++) {
        assert(add(arr1, &v1[i]) == true);
    }

    result = concat(arr1, arr2);
    assert(result != NULL);
    assert(result->size == 2);
    for (int i = 0; i < 2; i++) {
        double *val = (double *)get(result, i);
        assert(*val == v1[i]);
    }
    destroy_dynamic_array(result);

    // Normal concatenation of two non-empty arrays
    for (int i = 0; i < 3; i++) {
        assert(add(arr2, &v2[i]) == true);
    }

    result = concat(arr1, arr2);
    assert(result != NULL);
    assert(result->size == 5);

    double expected[] = {1.0, 2.0, 3.0, 4.0, 5.0};
    for (int i = 0; i < 5; i++) {
        double *val = (double *)get(result, i);
        assert(val != NULL);
        assert(*val == expected[i]);
    }

    destroy_dynamic_array(arr1);
    destroy_dynamic_array(arr2);
    destroy_dynamic_array(result);

    printf("OK\n");
}

// ============================================================================
// SORT() TESTS
// ============================================================================

// Test sort() with various input orders
void test_sort() {
    printf("Testing sort... ");

    const FieldInfo *fi = get_double_field_info();
    DynamicArray *arr = create_empty_dynamic_array(fi);
    assert(arr != NULL);

    // NULL array should fail
    errno = 0;
    assert(sort(NULL) == false);
    assert(errno == EINVAL);

    // Empty array should fail
    errno = 0;
    assert(sort(arr) == false);
    assert(errno == EINVAL);

    // Single element array
    double value = 5.0;
    assert(add(arr, &value) == true);
    assert(sort(arr) == true);
    double *v = (double *)get(arr, 0);
    assert(*v == 5.0);

    destroy_dynamic_array(arr);

    // Already sorted array
    arr = create_empty_dynamic_array(fi);
    double sorted[] = {1.0, 2.0, 3.0, 4.0, 5.0};
    for (int i = 0; i < 5; i++) {
        assert(add(arr, &sorted[i]) == true);
    }
    assert(sort(arr) == true);
    for (int i = 0; i < 5; i++) {
        double *val = (double *)get(arr, i);
        assert(*val == sorted[i]);
    }
    destroy_dynamic_array(arr);

    // Reverse sorted array
    arr = create_empty_dynamic_array(fi);
    double reverse[] = {5.0, 4.0, 3.0, 2.0, 1.0};
    for (int i = 0; i < 5; i++) {
        assert(add(arr, &reverse[i]) == true);
    }
    assert(sort(arr) == true);
    double expected[] = {1.0, 2.0, 3.0, 4.0, 5.0};
    for (int i = 0; i < 5; i++) {
        double *val = (double *)get(arr, i);
        assert(*val == expected[i]);
    }
    destroy_dynamic_array(arr);

    // Random order array
    arr = create_empty_dynamic_array(fi);
    double random[] = {5.0, 2.0, 8.0, 1.0, 9.0, 3.0};
    for (int i = 0; i < 6; i++) {
        assert(add(arr, &random[i]) == true);
    }
    assert(sort(arr) == true);
    double expected_random[] = {1.0, 2.0, 3.0, 5.0, 8.0, 9.0};
    for (int i = 0; i < 6; i++) {
        double *val = (double *)get(arr, i);
        assert(*val == expected_random[i]);
    }

    destroy_dynamic_array(arr);

    printf("OK\n");
}

// ============================================================================
// DESTROY_DYNAMIC_ARRAY() TESTS
// ============================================================================

// Test destroy_dynamic_array with edge cases
void test_destroy_dynamic_array() {
    printf("Testing destroy_dynamic_array... ");

    const FieldInfo *fi = get_double_field_info();

    // NULL array should fail
    errno = 0;
    assert(destroy_dynamic_array(NULL) == false);
    assert(errno == EINVAL);

    // Empty array should be destroyed successfully
    DynamicArray *arr = create_empty_dynamic_array(fi);
    assert(arr != NULL);
    assert(destroy_dynamic_array(arr) == true);

    // Array with elements should be destroyed successfully
    arr = create_empty_dynamic_array(fi);
    double values[] = {1.0, 2.0, 3.0, 4.0, 5.0};
    for (int i = 0; i < 5; i++) {
        assert(add(arr, &values[i]) == true);
    }
    assert(destroy_dynamic_array(arr) == true);

    // Array after multiple resizes should be destroyed successfully
    arr = create_dynamic_array(2, fi);
    for (int i = 0; i < 10; i++) {
        assert(add(arr, &values[i % 5]) == true);
    }
    assert(destroy_dynamic_array(arr) == true);

    printf("OK\n");
}

// ============================================================================
// TESTS WITH DIFFERENT DATA TYPES
// ============================================================================

// Test DynamicArray with double type
void test_double_type() {
    printf("Testing double type... ");

    const FieldInfo *fi = get_double_field_info();
    DynamicArray *arr = create_empty_dynamic_array(fi);
    assert(arr != NULL);

    double values[] = {3.14, 2.71, 1.41, 0.0, -1.5};
    for (int i = 0; i < 5; i++) {
        assert(add(arr, &values[i]) == true);
    }

    assert(arr->size == 5);
    assert(sort(arr) == true);

    double *v0 = (double *)get(arr, 0);
    double *v1 = (double *)get(arr, 1);
    double *v2 = (double *)get(arr, 2);
    double *v3 = (double *)get(arr, 3);
    double *v4 = (double *)get(arr, 4);

    assert(*v0 == -1.5);
    assert(*v1 == 0.0);
    assert(*v2 == 1.41);
    assert(*v3 == 2.71);
    assert(*v4 == 3.14);

    destroy_dynamic_array(arr);

    printf("OK\n");
}

// Test DynamicArray with string type (fixed-size char arrays)
void test_string_type() {
    printf("Testing string type... ");

    const FieldInfo *fi = get_string_field_info();
    DynamicArray *arr = create_empty_dynamic_array(fi);
    assert(arr != NULL);

    char str1[] = "hello";
    char str2[] = "world";
    char str3[] = "test";

    assert(add(arr, str1) == true);
    assert(add(arr, str2) == true);
    assert(add(arr, str3) == true);

    assert(arr->size == 3);

    char *s0 = (char *)get(arr, 0);
    char *s1 = (char *)get(arr, 1);
    char *s2 = (char *)get(arr, 2);

    assert(strcmp(s0, "hello") == 0);
    assert(strcmp(s1, "world") == 0);
    assert(strcmp(s2, "test") == 0);

    // Test string at maximum allowed length (49 chars + null terminator)
    char max_str[50];
    for (int i = 0; i < 49; i++) {
        max_str[i] = 'a';
    }
    max_str[49] = '\0';

    assert(add(arr, max_str) == true);
    char *s3 = (char *)get(arr, 3);
    assert(strlen(s3) == 49);
    assert(s3[49] == '\0');

    // Test string exceeding maximum length should be rejected
    char long_str[100];
    for (int i = 0; i < 99; i++) {
        long_str[i] = 'b';
    }
    long_str[99] = '\0';

    errno = 0;
    assert(add(arr, long_str) == false);  // Expected: fails because len > STRING_SIZE
    assert(errno == EINVAL);

    // Test string sorting
    destroy_dynamic_array(arr);
    arr = create_empty_dynamic_array(fi);

    char *strings[] = {"banana", "apple", "cherry"};
    for (int i = 0; i < 3; i++) {
        assert(add(arr, strings[i]) == true);
    }

    assert(sort(arr) == true);

    s0 = (char *)get(arr, 0);
    s1 = (char *)get(arr, 1);
    s2 = (char *)get(arr, 2);

    assert(strcmp(s0, "apple") == 0);
    assert(strcmp(s1, "banana") == 0);
    assert(strcmp(s2, "cherry") == 0);

    destroy_dynamic_array(arr);

    printf("OK\n");
}

// Test DynamicArray with Point type (sorted by distance from origin)
void test_point_type() {
    printf("Testing point type... ");

    const FieldInfo *fi = get_point_field_info();
    DynamicArray *arr = create_empty_dynamic_array(fi);
    assert(arr != NULL);

    Point p1 = {1.0, 1.0};     // dist^2 = 2
    Point p2 = {3.0, 4.0};     // dist^2 = 25
    Point p3 = {0.0, 0.0};     // dist^2 = 0
    Point p4 = {-1.0, -1.0};   // dist^2 = 2
    Point p5 = {5.0, 0.0};     // dist^2 = 25

    assert(add(arr, &p1) == true);
    assert(add(arr, &p2) == true);
    assert(add(arr, &p3) == true);
    assert(add(arr, &p4) == true);
    assert(add(arr, &p5) == true);

    assert(arr->size == 5);
    assert(sort(arr) == true);

    Point *r0 = (Point *)get(arr, 0);
    Point *r1 = (Point *)get(arr, 1);
    Point *r2 = (Point *)get(arr, 2);
    Point *r3 = (Point *)get(arr, 3);
    Point *r4 = (Point *)get(arr, 4);

    // Verify sorting by squared distance from origin
    assert(r0->x == 0.0 && r0->y == 0.0);                    // dist^2 = 0
    assert((r1->x == 1.0 && r1->y == 1.0) ||                // dist^2 = 2
           (r1->x == -1.0 && r1->y == -1.0));
    assert((r2->x == 1.0 && r2->y == 1.0) ||                // dist^2 = 2
           (r2->x == -1.0 && r2->y == -1.0));
    assert((r3->x == 3.0 && r3->y == 4.0) ||                // dist^2 = 25
           (r3->x == 5.0 && r3->y == 0.0));
    assert((r4->x == 3.0 && r4->y == 4.0) ||                // dist^2 = 25
           (r4->x == 5.0 && r4->y == 0.0));

    destroy_dynamic_array(arr);

    printf("OK\n");
}

// ============================================================================
// INTEGRATION TESTS
// ============================================================================

// Test chained operations: where -> map -> concat -> sort
void test_integration_chain() {
    printf("Testing integration chain... ");

    const FieldInfo *fi = get_double_field_info();
    DynamicArray *arr = create_empty_dynamic_array(fi);
    assert(arr != NULL);

    // Create array with values from -5 to 5
    for (int i = -5; i <= 5; i++) {
        double val = (double)i;
        assert(add(arr, &val) == true);
    }

    // Filter only positive numbers
    DynamicArray *positive = where(arr, is_positive);
    assert(positive != NULL);
    assert(positive->size == 5);

    // Multiply filtered values by 2
    DynamicArray *doubled = map(positive, multiply_by_two);
    assert(doubled != NULL);
    assert(doubled->size == 5);

    // Verify transformed values
    double expected[] = {2.0, 4.0, 6.0, 8.0, 10.0};
    for (int i = 0; i < 5; i++) {
        double *val = (double *)get(doubled, i);
        assert(*val == expected[i]);
    }

    // Concatenate original and transformed arrays
    DynamicArray *concatenated = concat(arr, doubled);
    assert(concatenated != NULL);
    assert(concatenated->size == 16);  // 11 original + 5 transformed

    // Sort the combined result
    assert(sort(concatenated) == true);

    // Verify first and last elements after sorting
    double *first = (double *)get(concatenated, 0);
    assert(*first == -5.0);

    double *last = (double *)get(concatenated, 15);
    assert(*last == 10.0);

    destroy_dynamic_array(arr);
    destroy_dynamic_array(positive);
    destroy_dynamic_array(doubled);
    destroy_dynamic_array(concatenated);

    printf("OK\n");
}

// Test memory handling with large number of elements
void test_memory_stress() {
    printf("Testing memory stress... ");

    const FieldInfo *fi = get_double_field_info();
    DynamicArray *arr = create_empty_dynamic_array(fi);
    assert(arr != NULL);

    // Add 1000 elements to trigger multiple resizes
    for (int i = 0; i < 1000; i++) {
        double val = (double)i;
        assert(add(arr, &val) == true);
    }

    assert(arr->size == 1000);
    assert(arr->data_capacity >= 1000);

    // Verify boundary elements
    double *first = (double *)get(arr, 0);
    double *last = (double *)get(arr, 999);
    assert(*first == 0.0);
    assert(*last == 999.0);

    // Verify middle element
    double *mid = (double *)get(arr, 500);
    assert(*mid == 500.0);

    destroy_dynamic_array(arr);

    printf("OK\n");
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main() {
    printf("Starting DynamicArray tests\n");

    // Run basic operation tests
    test_create_dynamic_array();
    test_create_empty_dynamic_array();
    test_add();
    test_get();
    test_set();
    test_map();
    test_where();
    test_concat();
    test_sort();
    test_destroy_dynamic_array();

    // Run type-specific tests
    test_double_type();
    test_string_type();
    test_point_type();

    // Run integration and stress tests
    test_integration_chain();
    test_memory_stress();

    printf("All tests passed successfully\n");

    return 0;
}