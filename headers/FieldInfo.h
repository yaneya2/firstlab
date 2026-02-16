//
// Created by Vasya on 07.02.2026.
//
#ifndef FIRSTLAB_FIELDINFO_H
#define FIRSTLAB_FIELDINFO_H
#include <windows.h>
#include "Allocator.h"
#define PTR_SIZE 8
typedef void (*UnaryOperator)(void *, void *);
typedef boolean (*BooleanOperator)(const void *, const void *);
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
#endif //FIRSTLAB_FIELDINFO_H