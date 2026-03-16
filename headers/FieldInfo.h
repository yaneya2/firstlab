#ifndef FIRSTLAB_FIELDINFO_H
#define FIRSTLAB_FIELDINFO_H

#include <stdbool.h>
#include "Allocator.h"

typedef bool (*UnaryOperator)(void *,const void *);
typedef int (*CompareOperator)(const void *, const void *);


typedef struct {
    int size;
    char *name;
    UnaryOperator assign;
    Allocator allocate;
    Deallocator deallocate;
    CompareOperator compare;
} FieldInfo;

const FieldInfo *getDoubleFieldInfo();
const FieldInfo *getStringFieldInfo();
const FieldInfo *getPointFieldInfo();

#endif //FIRSTLAB_FIELDINFO_H