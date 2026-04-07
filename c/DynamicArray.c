#include "../headers/DynamicArray.h"
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>

#define START_DATA_CAPACITY 10
#define GROW_FACTOR 2

DynamicArray *create_dynamic_array(size_t data_capacity, const FieldInfo *field_info) {
    if (field_info == NULL || data_capacity <= 0) {
        errno = EINVAL;
        return NULL;
    }

    DynamicArray *dynamic_array = (DynamicArray *)malloc(sizeof(DynamicArray));
    if (dynamic_array == NULL) {
        errno = ENOMEM;
        return NULL;
    }

    dynamic_array->data_capacity = data_capacity;
    dynamic_array->size = 0;
    dynamic_array->field_info = field_info;
    dynamic_array->data = malloc(data_capacity * field_info->size);
    if (dynamic_array->data == NULL) {
        errno = ENOMEM;
        free(dynamic_array);
        return NULL;
    }

    return dynamic_array;
}

DynamicArray *create_empty_dynamic_array(const FieldInfo *field_info) {
    return create_dynamic_array(START_DATA_CAPACITY, field_info);
}

bool add(DynamicArray *dynamic_array, const void *element) {
    if (dynamic_array == NULL || element == NULL) {
        errno = EINVAL;
        return false;
    }

    if (dynamic_array->size == dynamic_array->data_capacity) {
        size_t new_capacity = dynamic_array->data_capacity * GROW_FACTOR;
        void *temp_data_ptr = realloc(dynamic_array->data, new_capacity * dynamic_array->field_info->size);
        if (temp_data_ptr == NULL) {
            errno = ENOMEM;
            return false;
        }
        dynamic_array->data = temp_data_ptr;
        dynamic_array->data_capacity = new_capacity;
    }

    unsigned char *element_ptr = (unsigned char *)dynamic_array->data +
                                  dynamic_array->field_info->size * dynamic_array->size;

    if (!dynamic_array->field_info->allocate(element_ptr)) {
        return false;
    }

    if (!dynamic_array->field_info->assign(element_ptr, element)) {
        int saved_errno = errno;
        dynamic_array->field_info->deallocate(element_ptr);
        errno = saved_errno;
        return false;
    }

    dynamic_array->size++;
    return true;
}

void *get(const DynamicArray *dynamic_array, size_t index) {
    if (dynamic_array == NULL || index >= dynamic_array->size) {
        errno = EINVAL;
        return NULL;
    }

    return (unsigned char *)dynamic_array->data + dynamic_array->field_info->size * index;
}

bool set(DynamicArray *dynamic_array, size_t index, const void *value) {
    if (dynamic_array == NULL || index >= dynamic_array->size || value == NULL) {
        errno = EINVAL;
        return false;
    }

    void *element_ptr = get(dynamic_array, index);
    if (element_ptr == NULL) {
        return false;
    }

    if (!dynamic_array->field_info->deallocate(element_ptr)) {
        return false;
    }

    if (!dynamic_array->field_info->allocate(element_ptr)) {
        return false;
    }

    if (!dynamic_array->field_info->assign(element_ptr, value)) {
        return false;
    }

    return true;
}

bool map(const DynamicArray *dynamic_array, void *(*function)(void *), DynamicArray *result) {
    if (dynamic_array == NULL || function == NULL) {
        errno = EINVAL;
        return false;
    }

    if (result == NULL) {
        return false;
    }

    for (size_t i = 0; i < dynamic_array->size; ++i) {
        void *new_value = function(get(dynamic_array, i));
        if (new_value == NULL) {
            clear(result);
            return false;
        }
        if (!add(result, new_value)) {
            free(new_value);
            clear(result);
            return false;
        }
        free(new_value);
    }
    return true;
}

bool where(const DynamicArray *dynamic_array, bool (*function)(const void *), DynamicArray *result) {
    if (dynamic_array == NULL || function == NULL) {
        errno = EINVAL;
        return false;
    }

    if (result == NULL) {
        return false;
    }

    for (size_t i = 0; i < dynamic_array->size; ++i) {
        void *element = get(dynamic_array, i);
        if (element != NULL && function(element)) {
            if (!add(result, element)) {
                clear(result);
                return false;
            }
        }
    }

    return true;
}

bool concat(const DynamicArray *dynamic_array1, const DynamicArray *dynamic_array2, DynamicArray *result) {
    if (dynamic_array1 == NULL || dynamic_array2 == NULL) {
        errno = EINVAL;
        return false;
    }

    if (dynamic_array1->field_info != dynamic_array2->field_info) {
        errno = EINVAL;
        return false;
    }

    if (result == NULL) {
        return false;
    }

    for (size_t i = 0; i < dynamic_array1->size; ++i) {
        if (!add(result, get(dynamic_array1, i))) {
            clear(result);
            return false;
        }
    }

    for (size_t i = 0; i < dynamic_array2->size; ++i) {
        if (!add(result, get(dynamic_array2, i))) {
            clear(result);
            return false;
        }
    }

    return result;
}

bool sort(DynamicArray *dynamic_array) {
    if (dynamic_array == NULL || dynamic_array->size == 0) {
        errno = EINVAL;
        return false;
    }

    void *temp = malloc(dynamic_array->field_info->size);
    if (temp == NULL) {
        errno = ENOMEM;
        return false;
    }

    for (size_t i = 0; i < dynamic_array->size - 1; ++i) {
        size_t min_index = i;
        for (size_t j = i + 1; j < dynamic_array->size; ++j) {
            if (dynamic_array->field_info->compare(
                    get(dynamic_array, j),
                    get(dynamic_array, min_index)) < 0) {
                min_index = j;
            }
        }

        if (min_index != i) {
            void *elem_i = get(dynamic_array, i);
            void *elem_min = get(dynamic_array, min_index);

            if (!dynamic_array->field_info->assign(temp, elem_i) ||
                !dynamic_array->field_info->assign(elem_i, elem_min) ||
                !dynamic_array->field_info->assign(elem_min, temp)) {
                free(temp);
                return false;
            }
        }
    }

    free(temp);
    return true;
}

bool destroy_dynamic_array(DynamicArray *dynamic_array) {
    if (dynamic_array == NULL) {
        errno = EINVAL;
        return false;
    }

    if (dynamic_array->data != NULL && dynamic_array->field_info != NULL) {
        for (size_t i = 0; i < dynamic_array->size; ++i) {
            dynamic_array->field_info->deallocate(get(dynamic_array, i));
        }
        free(dynamic_array->data);
    }

    free(dynamic_array);
    return true;
}

bool clear(DynamicArray *dynamic_array) {
    if (dynamic_array == NULL) {
        errno = EINVAL;
        return false;
    }
    for (size_t i = 0; i < dynamic_array->size; ++i) {
        dynamic_array->field_info->deallocate(get(dynamic_array, i));
    }
    dynamic_array->size = 0;
    return true;
}