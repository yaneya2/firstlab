#ifndef FIRSTLAB_FIELD_INFO_H
#define FIRSTLAB_FIELD_INFO_H

#include <stdbool.h>
#include "Allocator.h"

typedef bool (*UnaryOperator)(void *, const void *);
typedef int (*CompareOperator)(const void *, const void *);

typedef struct {
    int size;
    char *name;
    UnaryOperator assign;
    Allocator allocate;
    Deallocator deallocate;
    CompareOperator compare;
} FieldInfo;

const FieldInfo *get_double_field_info(void);
const FieldInfo *get_string_field_info(void);
const FieldInfo *get_point_field_info(void);

#endif /* FIRSTLAB_FIELD_INFO_H */