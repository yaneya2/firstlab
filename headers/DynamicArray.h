#ifndef FIRSTLAB_DYNAMIC_ARRAY_H
#define FIRSTLAB_DYNAMIC_ARRAY_H

#include "FieldInfo.h"
#include <stdbool.h>

typedef struct {
    int data_capacity;
    int size;
    const FieldInfo *field_info;
    void *data;
} DynamicArray;

DynamicArray *create_dynamic_array(int data_capacity, const FieldInfo *field_info);
DynamicArray *create_empty_dynamic_array(const FieldInfo *field_info);
bool add(DynamicArray *dynamic_array, const void *element);
void *get(const DynamicArray *dynamic_array, int index);
bool set(DynamicArray *dynamic_array, int index, const void *value);
DynamicArray *map(const DynamicArray *dynamic_array, void *(*function)(void *));
DynamicArray *where(const DynamicArray *dynamic_array, bool (*function)(const void *));
DynamicArray *concat(const DynamicArray *dynamic_array1, const DynamicArray *dynamic_array2);
bool sort(DynamicArray *dynamic_array);
bool destroy_dynamic_array(DynamicArray *dynamic_array);

#endif /* FIRSTLAB_DYNAMIC_ARRAY_H */