#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include "headers/DynamicArray.h"
#include "headers/FieldInfo.h"
#include "headers/UsersStruct.h"

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

bool always_true(const void *x) {
    (void)x;
    return true;
}

bool always_false(const void *x) {
    (void)x;
    return false;
}

bool is_positive(const void *x) {
    const double *val = (const double *)x;
    return *val > 0;
}

bool is_negative(const void *x) {
    const double *val = (const double *)x;
    return *val < 0;
}

void test_create_dynamic_array() {
    printf("Testing create_dynamic_array... ");

    const FieldInfo *fi = get_double_field_info();
    assert(fi != NULL);

    DynamicArray *arr = create_dynamic_array(10, fi);
    assert(arr != NULL);
    assert(arr->size == 0);
    assert(arr->data_capacity == 10);
    assert(arr->field_info == fi);
    assert(arr->data != NULL);
    destroy_dynamic_array(arr);

    errno = 0;
    arr = create_dynamic_array(10, NULL);
    assert(arr == NULL);
    assert(errno == EINVAL);

    errno = 0;
    arr = create_dynamic_array(0, fi);
    assert(arr == NULL);
    assert(errno == EINVAL);

    printf("OK\n");
}

void test_create_empty_dynamic_array() {
    printf("Testing create_empty_dynamic_array... ");

    const FieldInfo *fi = get_string_field_info();
    assert(fi != NULL);

    DynamicArray *arr = create_empty_dynamic_array(fi);
    assert(arr != NULL);
    assert(arr->size == 0);
    assert(arr->data_capacity == 10);
    assert(arr->field_info == fi);
    assert(arr->data != NULL);
    destroy_dynamic_array(arr);

    errno = 0;
    arr = create_empty_dynamic_array(NULL);
    assert(arr == NULL);
    assert(errno == EINVAL);

    printf("OK\n");
}

void test_add() {
    printf("Testing add... ");

    const FieldInfo *fi = get_double_field_info();
    DynamicArray *arr = create_empty_dynamic_array(fi);
    assert(arr != NULL);

    errno = 0;
    double val = 1.0;
    assert(add(NULL, &val) == false);
    assert(errno == EINVAL);

    errno = 0;
    assert(add(arr, NULL) == false);
    assert(errno == EINVAL);

    double value1 = 3.14;
    assert(add(arr, &value1) == true);
    assert(arr->size == 1);

    double *retrieved = (double *)get(arr, 0);
    assert(retrieved != NULL);
    assert(*retrieved == 3.14);

    for (int i = 0; i < 9; i++) {
        double v = (double)i;
        assert(add(arr, &v) == true);
    }
    assert(arr->size == 10);
    assert(arr->data_capacity == 10);

    double value2 = 2.71;
    assert(add(arr, &value2) == true);
    assert(arr->size == 11);
    assert(arr->data_capacity == 20);

    retrieved = (double *)get(arr, 10);
    assert(retrieved != NULL);
    assert(*retrieved == 2.71);

    retrieved = (double *)get(arr, 0);
    assert(retrieved != NULL);
    assert(*retrieved == 3.14);

    destroy_dynamic_array(arr);

    printf("OK\n");
}

void test_get() {
    printf("Testing get... ");

    const FieldInfo *fi = get_double_field_info();
    DynamicArray *arr = create_empty_dynamic_array(fi);
    assert(arr != NULL);

    errno = 0;
    assert(get(NULL, 0) == NULL);
    assert(errno == EINVAL);

    errno = 0;
    assert(get(arr, -1) == NULL);
    assert(errno == EINVAL);

    errno = 0;
    assert(get(arr, 0) == NULL);
    assert(errno == EINVAL);

    double values[] = {1.41, 2.71, 3.14};
    for (int i = 0; i < 3; i++) {
        assert(add(arr, &values[i]) == true);
    }

    for (int i = 0; i < 3; i++) {
        double *retrieved = (double *)get(arr, i);
        assert(retrieved != NULL);
        assert(*retrieved == values[i]);
    }

    errno = 0;
    assert(get(arr, 3) == NULL);
    assert(errno == EINVAL);

    destroy_dynamic_array(arr);

    printf("OK\n");
}

void test_set() {
    printf("Testing set... ");

    const FieldInfo *fi = get_double_field_info();
    DynamicArray *arr = create_empty_dynamic_array(fi);
    assert(arr != NULL);

    errno = 0;
    double val = 1.0;
    assert(set(NULL, 0, &val) == false);
    assert(errno == EINVAL);

    errno = 0;
    assert(set(arr, 0, NULL) == false);
    assert(errno == EINVAL);

    errno = 0;
    assert(set(arr, 0, &val) == false);
    assert(errno == EINVAL);

    double value1 = 3.14;
    double value2 = 2.71;
    double value3 = 1.41;
    assert(add(arr, &value1) == true);
    assert(add(arr, &value2) == true);

    assert(set(arr, 0, &value3) == true);
    double *retrieved = (double *)get(arr, 0);
    assert(*retrieved == 1.41);

    retrieved = (double *)get(arr, 1);
    assert(*retrieved == 2.71);

    errno = 0;
    assert(set(arr, -1, &value1) == false);
    assert(errno == EINVAL);

    errno = 0;
    assert(set(arr, 5, &value1) == false);
    assert(errno == EINVAL);

    destroy_dynamic_array(arr);

    printf("OK\n");
}

void test_map() {
    printf("Testing map... ");

    const FieldInfo *fi = get_double_field_info();
    DynamicArray *arr = create_empty_dynamic_array(fi);
    DynamicArray *result = create_empty_dynamic_array(fi);
    assert(arr != NULL);
    assert(result != NULL);

    errno = 0;
    assert(map(NULL, multiply_by_two, result) == false);
    assert(errno == EINVAL);

    errno = 0;
    assert(map(arr, NULL, result) == false);
    assert(errno == EINVAL);

    assert(map(arr, multiply_by_two, NULL) == false);

    assert(map(arr, multiply_by_two, result) == true);
    assert(result->size == 0);
    assert(result->data_capacity == 10);
    destroy_dynamic_array(result);

    double values[] = {1.0, 2.0, 3.0, 4.0, 5.0};
    for (int i = 0; i < 5; i++) {
        assert(add(arr, &values[i]) == true);
    }

    result = create_empty_dynamic_array(fi);
    assert(result != NULL);
    assert(map(arr, multiply_by_two, result) == true);
    assert(result->size == 5);

    for (int i = 0; i < 5; i++) {
        double *r = (double *)get(result, i);
        assert(r != NULL);
        assert(*r == values[i] * 2.0);
    }

    for (int i = 0; i < 5; i++) {
        double *orig = (double *)get(arr, i);
        assert(*orig == values[i]);
    }

    destroy_dynamic_array(arr);
    destroy_dynamic_array(result);

    arr = create_empty_dynamic_array(fi);
    result = create_empty_dynamic_array(fi);
    assert(arr != NULL);
    assert(result != NULL);

    double identity_values[] = {10.0, 20.0, 30.0};
    for (int i = 0; i < 3; i++) {
        assert(add(arr, &identity_values[i]) == true);
    }

    assert(map(arr, identity_function, result) == true);
    assert(result->size == 3);

    for (int i = 0; i < 3; i++) {
        double *r = (double *)get(result, i);
        assert(r != NULL);
        assert(*r == identity_values[i]);
    }

    destroy_dynamic_array(arr);
    destroy_dynamic_array(result);

    arr = create_empty_dynamic_array(fi);
    result = create_empty_dynamic_array(fi);
    assert(arr != NULL);
    assert(result != NULL);

    double test_val = 5.0;
    assert(add(arr, &test_val) == true);

    errno = 0;
    assert(map(arr, NULL, result) == false);
    assert(errno == EINVAL);

    destroy_dynamic_array(arr);
    destroy_dynamic_array(result);

    printf("OK\n");
}

void test_where() {
    printf("Testing where... ");

    const FieldInfo *fi = get_double_field_info();
    DynamicArray *arr = create_empty_dynamic_array(fi);
    DynamicArray *result = create_empty_dynamic_array(fi);
    assert(arr != NULL);
    assert(result != NULL);

    errno = 0;
    assert(where(NULL, is_positive, result) == false);
    assert(errno == EINVAL);

    errno = 0;
    assert(where(arr, NULL, result) == false);
    assert(errno == EINVAL);

    assert(where(arr, always_true, NULL) == false);

    assert(where(arr, always_true, result) == true);
    assert(result->size == 0);
    destroy_dynamic_array(result);

    double values[] = {1.0, 2.0, 3.0};
    for (int i = 0; i < 3; i++) {
        assert(add(arr, &values[i]) == true);
    }

    result = create_empty_dynamic_array(fi);
    assert(result != NULL);
    assert(where(arr, always_true, result) == true);
    assert(result->size == 3);
    destroy_dynamic_array(result);

    result = create_empty_dynamic_array(fi);
    assert(result != NULL);
    assert(where(arr, always_false, result) == true);
    assert(result->size == 0);
    destroy_dynamic_array(result);

    destroy_dynamic_array(arr);
    arr = create_empty_dynamic_array(fi);
    assert(arr != NULL);

    double filter_values[] = {-1.0, 2.0, -3.0, 4.0, -5.0, 6.0};
    for (int i = 0; i < 6; i++) {
        assert(add(arr, &filter_values[i]) == true);
    }

    result = create_empty_dynamic_array(fi);
    assert(result != NULL);
    assert(where(arr, is_positive, result) == true);
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

void test_concat() {
    printf("Testing concat... ");

    const FieldInfo *fi = get_double_field_info();
    DynamicArray *arr1 = create_empty_dynamic_array(fi);
    DynamicArray *arr2 = create_empty_dynamic_array(fi);
    DynamicArray *result = create_empty_dynamic_array(fi);
    assert(arr1 != NULL);
    assert(arr2 != NULL);
    assert(result != NULL);

    errno = 0;
    assert(concat(NULL, arr2, result) == false);
    assert(errno == EINVAL);

    errno = 0;
    assert(concat(arr1, NULL, result) == false);
    assert(errno == EINVAL);

    assert(concat(arr1, arr2, NULL) == false);

    const FieldInfo *fi_string = get_string_field_info();
    DynamicArray *arr3 = create_empty_dynamic_array(fi_string);
    assert(arr3 != NULL);
    errno = 0;
    assert(concat(arr1, arr3, result) == false);
    assert(errno == EINVAL);
    destroy_dynamic_array(arr3);

    assert(concat(arr1, arr2, result) == true);
    assert(result->size == 0);
    destroy_dynamic_array(result);

    double v2[] = {3.0, 4.0, 5.0};
    for (int i = 0; i < 3; i++) {
        assert(add(arr2, &v2[i]) == true);
    }

    result = create_empty_dynamic_array(fi);
    assert(result != NULL);
    assert(concat(arr1, arr2, result) == true);
    assert(result->size == 3);
    for (int i = 0; i < 3; i++) {
        double *val = (double *)get(result, i);
        assert(*val == v2[i]);
    }
    destroy_dynamic_array(result);

    destroy_dynamic_array(arr2);
    arr2 = create_empty_dynamic_array(fi);
    assert(arr2 != NULL);

    double v1[] = {1.0, 2.0};
    for (int i = 0; i < 2; i++) {
        assert(add(arr1, &v1[i]) == true);
    }

    result = create_empty_dynamic_array(fi);
    assert(result != NULL);
    assert(concat(arr1, arr2, result) == true);
    assert(result->size == 2);
    for (int i = 0; i < 2; i++) {
        double *val = (double *)get(result, i);
        assert(*val == v1[i]);
    }
    destroy_dynamic_array(result);

    for (int i = 0; i < 3; i++) {
        assert(add(arr2, &v2[i]) == true);
    }

    result = create_empty_dynamic_array(fi);
    assert(result != NULL);
    assert(concat(arr1, arr2, result) == true);
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

void test_sort() {
    printf("Testing sort... ");

    const FieldInfo *fi = get_double_field_info();
    DynamicArray *arr = create_empty_dynamic_array(fi);
    assert(arr != NULL);

    errno = 0;
    assert(sort(NULL) == false);
    assert(errno == EINVAL);

    errno = 0;
    assert(sort(arr) == false);
    assert(errno == EINVAL);

    double value = 5.0;
    assert(add(arr, &value) == true);
    assert(sort(arr) == true);
    double *v = (double *)get(arr, 0);
    assert(*v == 5.0);

    destroy_dynamic_array(arr);

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

void test_destroy_dynamic_array() {
    printf("Testing destroy_dynamic_array... ");

    const FieldInfo *fi = get_double_field_info();

    errno = 0;
    assert(destroy_dynamic_array(NULL) == false);
    assert(errno == EINVAL);

    DynamicArray *arr = create_empty_dynamic_array(fi);
    assert(arr != NULL);
    assert(destroy_dynamic_array(arr) == true);

    arr = create_empty_dynamic_array(fi);
    double values[] = {1.0, 2.0, 3.0, 4.0, 5.0};
    for (int i = 0; i < 5; i++) {
        assert(add(arr, &values[i]) == true);
    }
    assert(destroy_dynamic_array(arr) == true);

    arr = create_dynamic_array(2, fi);
    for (int i = 0; i < 10; i++) {
        assert(add(arr, &values[i % 5]) == true);
    }
    assert(destroy_dynamic_array(arr) == true);

    printf("OK\n");
}

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

    char max_str[50];
    for (int i = 0; i < 49; i++) {
        max_str[i] = 'a';
    }
    max_str[49] = '\0';

    assert(add(arr, max_str) == true);
    char *s3 = (char *)get(arr, 3);
    assert(strlen(s3) == 49);
    assert(s3[49] == '\0');

    char long_str[100];
    for (int i = 0; i < 99; i++) {
        long_str[i] = 'b';
    }
    long_str[99] = '\0';

    errno = 0;
    assert(add(arr, long_str) == false);
    assert(errno == EINVAL);

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

void test_point_type() {
    printf("Testing point type... ");

    const FieldInfo *fi = get_point_field_info();
    DynamicArray *arr = create_empty_dynamic_array(fi);
    assert(arr != NULL);

    Point p1 = {1.0, 1.0};
    Point p2 = {3.0, 4.0};
    Point p3 = {0.0, 0.0};
    Point p4 = {-1.0, -1.0};
    Point p5 = {5.0, 0.0};

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

    assert(r0->x == 0.0 && r0->y == 0.0);
    assert((r1->x == 1.0 && r1->y == 1.0) ||
           (r1->x == -1.0 && r1->y == -1.0));
    assert((r2->x == 1.0 && r2->y == 1.0) ||
           (r2->x == -1.0 && r2->y == -1.0));
    assert((r3->x == 3.0 && r3->y == 4.0) ||
           (r3->x == 5.0 && r3->y == 0.0));
    assert((r4->x == 3.0 && r4->y == 4.0) ||
           (r4->x == 5.0 && r4->y == 0.0));

    destroy_dynamic_array(arr);

    printf("OK\n");
}

void test_integration_chain() {
    printf("Testing integration chain... ");

    const FieldInfo *fi = get_double_field_info();
    DynamicArray *arr = create_empty_dynamic_array(fi);
    DynamicArray *positive = create_empty_dynamic_array(fi);
    DynamicArray *doubled = create_empty_dynamic_array(fi);
    DynamicArray *concatenated = create_empty_dynamic_array(fi);
    assert(arr != NULL);
    assert(positive != NULL);
    assert(doubled != NULL);
    assert(concatenated != NULL);

    for (int i = -5; i <= 5; i++) {
        double val = (double)i;
        assert(add(arr, &val) == true);
    }

    assert(where(arr, is_positive, positive) == true);
    assert(positive->size == 5);

    assert(map(positive, multiply_by_two, doubled) == true);
    assert(doubled->size == 5);

    double expected[] = {2.0, 4.0, 6.0, 8.0, 10.0};
    for (int i = 0; i < 5; i++) {
        double *val = (double *)get(doubled, i);
        assert(*val == expected[i]);
    }

    assert(concat(arr, doubled, concatenated) == true);
    assert(concatenated->size == 16);

    assert(sort(concatenated) == true);

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

void test_memory_stress() {
    printf("Testing memory stress... ");

    const FieldInfo *fi = get_double_field_info();
    DynamicArray *arr = create_empty_dynamic_array(fi);
    assert(arr != NULL);

    for (int i = 0; i < 1000; i++) {
        double val = (double)i;
        assert(add(arr, &val) == true);
    }

    assert(arr->size == 1000);
    assert(arr->data_capacity >= 1000);

    double *first = (double *)get(arr, 0);
    double *last = (double *)get(arr, 999);
    assert(*first == 0.0);
    assert(*last == 999.0);

    double *mid = (double *)get(arr, 500);
    assert(*mid == 500.0);

    destroy_dynamic_array(arr);

    printf("OK\n");
}

int main() {
    printf("Starting DynamicArray tests\n");

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
    test_double_type();
    test_string_type();
    test_point_type();
    test_integration_chain();
    test_memory_stress();

    printf("All tests passed successfully\n");

    return 0;
}
