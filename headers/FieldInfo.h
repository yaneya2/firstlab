//
// Created by Vasya on 07.02.2026.
//

#ifndef FIRSTLAB_FIELDINFO_H
#define FIRSTLAB_FIELDINFO_H
#include <stdlib.h>
#include <string.h>

#include "Allocator.h"
#define PTR_SIZE 8
typedef void (*UnaryOperator)(void * res, void * arg);
typedef int boolean;
#define TRUE 1
#define FALSE 0
typedef int (*BooleanOperator)(const void *, const void *);

typedef struct {
    int size;
    char *name;
    //Unused field
    //char *ToString;
    int (*compare)(const void *, const void *);
    char * (*toString)(const void *);
} FieldInfo;
const FieldInfo *getDoubleFieldInfo();
const FieldInfo *getStringFieldInfo();
#endif //FIRSTLAB_FIELDINFO_H