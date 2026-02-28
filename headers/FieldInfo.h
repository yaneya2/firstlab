#ifndef FIRSTLAB_FIELDINFO_H
#define FIRSTLAB_FIELDINFO_H

#include <stdbool.h>
#include "Allocator.h"

typedef void (*UnaryOperator)(void *, void *);
typedef bool (*BooleanOperator)(const void *, const void *);
typedef char * (*StringOperator)(const void *);

typedef struct {
    int size;
    char *name;
    UnaryOperator assign;
    Allocator allocate;
    Deallocator deallocate;
    BooleanOperator compare;
} FieldInfo;

const FieldInfo *getDoubleFieldInfo();
const FieldInfo *getStringFieldInfo();
const FieldInfo *getPointFieldInfo();

#endif //FIRSTLAB_FIELDINFO_H