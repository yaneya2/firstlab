#include "../headers/DynamicArray.h"
#include <stddef.h>
#include <stdlib.h>

DynamicArray * createDynamicArray(int data_capacity, const FieldInfo *field_info) {
    if (field_info == NULL || data_capacity <= 0) return NULL;
    DynamicArray *dynamicArray = (DynamicArray*)malloc(sizeof(DynamicArray));
    dynamicArray->data_capacity = data_capacity;
    dynamicArray->size = 0;
    dynamicArray->field_info = field_info;
    dynamicArray->data = malloc(dynamicArray->data_capacity * field_info->size);
    return dynamicArray;
}
DynamicArray * createEmptyDynamicArray(const FieldInfo *field_info) {
    if (field_info == NULL) return NULL;
    DynamicArray * dynamicArray = (DynamicArray*)malloc(sizeof(DynamicArray));
    dynamicArray->data_capacity = START_DATA_CAPACITY;
    dynamicArray->size = 0;
    dynamicArray->field_info = field_info;
    dynamicArray->data = malloc(dynamicArray->data_capacity * field_info->size);
    return dynamicArray;
}
void add(DynamicArray *dynamicArray,const void *ell) {
    if (dynamicArray == NULL || ell == NULL) return;
    if (dynamicArray->size == dynamicArray->data_capacity) {
        dynamicArray->data_capacity *= 2;
        dynamicArray->data = realloc(dynamicArray->data, dynamicArray->data_capacity * dynamicArray->field_info->size);
    }
    dynamicArray->data[dynamicArray->size] = dynamicArray->field_info->allocate();
    dynamicArray->field_info->assign(dynamicArray->data[dynamicArray->size], ell);
    dynamicArray->size++;

}
void * get(const DynamicArray *dynamicArray, int index) {
    if (dynamicArray == NULL || index < 0 || index >= dynamicArray->size) return NULL;
    return dynamicArray->data[index];
}
void map(DynamicArray *dynamicArray, void *(*function)(void *)) {
    for (int i = 0; i < dynamicArray->size; ++i) {
        dynamicArray->data[i] = function(dynamicArray->data[i]);
    }
}
void where(DynamicArray *dynamicArray, bool (*function)(const void *)) {
    for (int i = 0; i < dynamicArray->size; ++i) {
        if (!function(get(dynamicArray, i))) {
            dynamicArray->field_info->deallocate(get(dynamicArray, i));
            for (int j = i; j < dynamicArray->size - 1; ++j) {
                dynamicArray->data[j] = dynamicArray->data[j + 1];
            }
            dynamicArray->field_info->deallocate(get(dynamicArray, dynamicArray->size));
            dynamicArray->size--;
            i--;
        }
    }
}
DynamicArray * concat(const DynamicArray *dynamicArray1, const DynamicArray *dynamicArray2) {
    if (dynamicArray1 == NULL || dynamicArray2 == NULL) return NULL;
    if (dynamicArray1->field_info != dynamicArray2->field_info) return NULL;
    DynamicArray * dynamicArray = createDynamicArray(dynamicArray1->data_capacity + dynamicArray2->data_capacity, dynamicArray1->field_info);
    for (int i = 0; i < dynamicArray1->size; ++i) {
        add(dynamicArray, get(dynamicArray1, i));
    }
    for (int i = 0; i < dynamicArray2->size; ++i) {
        add(dynamicArray, get(dynamicArray2, i));
    }
    return dynamicArray;
}
void sort(DynamicArray *dynamicArray) {
    if (dynamicArray == NULL || dynamicArray->size == 0) return;
    int minIndex = 0;
    void *t = 0;
    for (int i = 0; i < dynamicArray->size - 1; i++) {
        minIndex = i;
        for (int j = i + 1; j < dynamicArray->size; j++) {
            if (!dynamicArray->field_info->compare(get(dynamicArray, j), get(dynamicArray, minIndex))){
                minIndex = j;
            }
        }
        t = get(dynamicArray, i);
        dynamicArray->data[i] = get(dynamicArray, minIndex);
        dynamicArray->data[minIndex] = t;
    }
}
void deleteDynamicArray(DynamicArray *dynamicArray) {
    if (dynamicArray == NULL) return;
    for (int i = 0; i < dynamicArray->size; i++) {
        dynamicArray->field_info->deallocate(get(dynamicArray, i));
    }
    free(dynamicArray->data);
    free(dynamicArray);
}
