#include "../headers/DynamicArray.h"
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>

#define GROW_FACTOR 2

DynamicArray * createDynamicArray(const int data_capacity, const FieldInfo *field_info) {
    if (field_info == NULL || data_capacity <= 0) {
        errno = EINVAL;
        return NULL;
    }
    DynamicArray *dynamicArray = (DynamicArray*)malloc(sizeof(DynamicArray));
    if (dynamicArray == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    dynamicArray->data_capacity = data_capacity;
    dynamicArray->size = 0;
    dynamicArray->field_info = field_info;
    dynamicArray->data = malloc(dynamicArray->data_capacity * field_info->size);
    if (dynamicArray->data == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    return dynamicArray;
}
DynamicArray * createEmptyDynamicArray(const FieldInfo *field_info) {
    if (field_info == NULL) {
        errno = EINVAL;
        return NULL;
    }
    DynamicArray * dynamicArray = (DynamicArray*)malloc(sizeof(DynamicArray));
    if (dynamicArray == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    dynamicArray->data_capacity = START_DATA_CAPACITY;
    dynamicArray->size = 0;
    dynamicArray->field_info = field_info;
    dynamicArray->data = malloc(dynamicArray->data_capacity * field_info->size);
    if (dynamicArray->data == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    return dynamicArray;
}
bool add(DynamicArray *dynamicArray,const void *ell) {
    if (dynamicArray == NULL || ell == NULL) {
        errno = EINVAL;
        return false;
    }
    if (dynamicArray->size == dynamicArray->data_capacity) {
        void * temp_data_ptr = realloc(dynamicArray->data, GROW_FACTOR * dynamicArray->data_capacity * dynamicArray->field_info->size);
        if (temp_data_ptr == NULL) {
            errno = ENOMEM;
            return false;
        }
        dynamicArray->data = temp_data_ptr;
        dynamicArray->data_capacity *= GROW_FACTOR;
    }
    if (!dynamicArray->field_info->allocate((unsigned char *) dynamicArray->data + dynamicArray->field_info->size * dynamicArray->size)) {
        return false;
    }
    if (!dynamicArray->field_info->assign((unsigned char *) dynamicArray->data + dynamicArray->field_info->size * dynamicArray->size, ell)) {
        int temp_errno = errno;
        dynamicArray->field_info->deallocate((unsigned char *)dynamicArray->data + dynamicArray->field_info->size * dynamicArray->size);
        errno = temp_errno;
        return false;
    }
    dynamicArray->size++;
    return true;
}
void * get(const DynamicArray *dynamicArray, const int index) {
    if (dynamicArray == NULL || index < 0 || index >= dynamicArray->size) {
        errno = EINVAL;
        return NULL;
    }
    return (unsigned char *)dynamicArray->data + index * dynamicArray->field_info->size;
}
bool set(DynamicArray *dynamicArray, const int index, const void *value) {
    if (dynamicArray == NULL || index < 0 || index >= dynamicArray->size) {
        errno = EINVAL;
        return false;
    }
    if (!dynamicArray->field_info->deallocate(get(dynamicArray, index))) {
        return false;
    }
    if (!dynamicArray->field_info->allocate(get(dynamicArray, index))) {
        return false;
    }
    if (!dynamicArray->field_info->assign(get(dynamicArray, index), value)) {
        return false;
    }
    return true;
}
bool map(DynamicArray * dynamicArray, void *(*function)(void *)) {
    if (dynamicArray == NULL || function == NULL) {
        errno = EINVAL;
        return false;
    }
    for (int i = 0; i < dynamicArray->size; ++i) {
        if (!set(dynamicArray, i,function(get(dynamicArray, i)))) {
            return false;
        }
    }
    return true;
}
bool where(DynamicArray *dynamicArray, bool (*function)(const void *)) {
    if (dynamicArray == NULL || function == NULL) {
        errno = EINVAL;
        return false;
    }
    for (int i = 0; i < dynamicArray->size; ++i) {
        if (!function(get(dynamicArray, i))) {
            dynamicArray->field_info->deallocate(get(dynamicArray, i));
            for (int j = i; j < dynamicArray->size - 1; ++j) {
                if (!dynamicArray->field_info->assign(get(dynamicArray, j ), get(dynamicArray, j + 1))) return false;
            }
            dynamicArray->field_info->deallocate(get(dynamicArray, dynamicArray->size));
            dynamicArray->size--;
            i--;
        }
    }
    return true;
}
DynamicArray * concat(const DynamicArray *dynamicArray1, const DynamicArray *dynamicArray2) {
    if (dynamicArray1 == NULL || dynamicArray2 == NULL) {
        errno = EINVAL;
        return NULL;
    }
    if (dynamicArray1->field_info != dynamicArray2->field_info) {
        errno = EINVAL;
        return NULL;
    }
    DynamicArray * dynamicArray = createDynamicArray(dynamicArray1->data_capacity + dynamicArray2->data_capacity, dynamicArray1->field_info);
    if (dynamicArray == NULL) {
        return NULL;
    }
    for (int i = 0; i < dynamicArray1->size; ++i) {
        if (!add(dynamicArray, get(dynamicArray1, i))) {
            deleteDynamicArray(dynamicArray);
            return NULL;
        }
    }
    for (int i = 0; i < dynamicArray2->size; ++i) {
        if (!add(dynamicArray, get(dynamicArray2, i))) {
            deleteDynamicArray(dynamicArray);
            return NULL;
        }
    }
    return dynamicArray;
}
bool sort(DynamicArray *dynamicArray) {
    if (dynamicArray == NULL || dynamicArray->size == 0) {
        errno = EINVAL;
        return false;
    }
    int minIndex = 0;
    void *t = malloc(dynamicArray->field_info->size);
    if (t == NULL) {
        errno = ENOMEM;
        return false;
    }
    for (int i = 0; i < dynamicArray->size - 1; i++) {
        minIndex = i;
        for (int j = i + 1; j < dynamicArray->size; j++) {
            if (dynamicArray->field_info->compare(get(dynamicArray, j), get(dynamicArray, minIndex)) == -1){
                minIndex = j;
            }
        }
        if (!dynamicArray->field_info->assign(t, get(dynamicArray, i)) ||
            !dynamicArray->field_info->assign(get(dynamicArray, i), get(dynamicArray, minIndex)) ||
            !dynamicArray->field_info->assign(get(dynamicArray, minIndex), t)) {
            return false;
        }
    }
    free(t);
    return true;
}
bool deleteDynamicArray(DynamicArray *dynamicArray) {
    if (dynamicArray == NULL) {
        errno = EINVAL;
        return false;
    }
    for (int i = 0; i < dynamicArray->size; i++) {
        dynamicArray->field_info->deallocate(get(dynamicArray, i));
    }
    free(dynamicArray->data);
    free(dynamicArray);
    return true;
}
