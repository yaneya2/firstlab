#ifndef FIRSTLAB_DynamicArray_H
#define FIRSTLAB_DynamicArray_H

#include "FieldInfo.h"

#define START_DATA_CAPACITY 10

typedef struct {
    int data_capacity;
    int size;
    FieldInfo *field_info;
    void **data;
}DynamicArray;

DynamicArray * createDynamicArray(int data_capacity, const FieldInfo *field_info);
DynamicArray * createEmptyDynamicArray(const FieldInfo *field_info);
void add(DynamicArray *dynamicArray,const void *ell);
void * get(const DynamicArray *dynamicArray, int index);
void map(DynamicArray *dynamicArray, void * (*function)(void *));
void where(DynamicArray *dynamicArray, bool (*function)(const void *));
DynamicArray * concat(const DynamicArray *dynamicArray1, const DynamicArray *dynamicArray2);
void sort(DynamicArray *dynamicArray);
void deleteDynamicArray(DynamicArray *dynamicArray);

#endif //FIRSTLAB_DynamicArray_H