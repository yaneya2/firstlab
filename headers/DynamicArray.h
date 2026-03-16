#ifndef FIRSTLAB_DynamicArray_H
#define FIRSTLAB_DynamicArray_H

#include "FieldInfo.h"

typedef struct {
    int data_capacity;
    int size;
    const FieldInfo *field_info;
    void *data;
} DynamicArray;

DynamicArray * createDynamicArray(int data_capacity, const FieldInfo *field_info);
DynamicArray * createEmptyDynamicArray(const FieldInfo *field_info);
bool add(DynamicArray *dynamicArray, const void *ell);
void * get(const DynamicArray *dynamicArray, int index);
DynamicArray * map(DynamicArray *dynamicArray, void *(*function)(void *));
DynamicArray * where(DynamicArray *dynamicArray, bool (*function)(const void *));
DynamicArray * concat(const DynamicArray *dynamicArray1, const DynamicArray *dynamicArray2);
bool sort(DynamicArray *dynamicArray);
bool destroyDynamicArray(DynamicArray *dynamicArray);

#endif //FIRSTLAB_DynamicArray_H