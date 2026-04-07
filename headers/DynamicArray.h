#ifndef FIRSTLAB_DYNAMIC_ARRAY_H
#define FIRSTLAB_DYNAMIC_ARRAY_H

#include "FieldInfo.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    size_t data_capacity;
    size_t size;
    const FieldInfo *field_info;
    void *data;
} DynamicArray;

DynamicArray *create_dynamic_array(size_t data_capacity, const FieldInfo *field_info);
DynamicArray *create_empty_dynamic_array(const FieldInfo *field_info);
bool add(DynamicArray *dynamic_array, const void *element);
void *get(const DynamicArray *dynamic_array, size_t index);
bool set(DynamicArray *dynamic_array, size_t index, const void *value);
bool map(const DynamicArray *dynamic_array, void *(*function)(void *), DynamicArray * result);
bool where(const DynamicArray *dynamic_array, bool (*function)(const void *), DynamicArray * result);
bool concat(const DynamicArray *dynamic_array1, const DynamicArray *dynamic_array2, DynamicArray * result);
bool sort(DynamicArray *dynamic_array);
bool destroy_dynamic_array(DynamicArray *dynamic_array);
bool clear(DynamicArray *dynamic_array);

#endif /* FIRSTLAB_DYNAMIC_ARRAY_H */