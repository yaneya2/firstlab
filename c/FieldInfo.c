#include <stddef.h>
#include "../headers/FieldInfo.h"
#include <windows.h>
#define STRING_SIZE 50
FieldInfo *DOUBLE_FIELD_INFO = NULL;
FieldInfo *STRING_FIELD_INFO = NULL;

boolean doubleCompare(const void *a, const void *b) {
    if (a == NULL || b == NULL) return FALSE;
    double *double_a = (double *)a;
    double *double_b = (double *)b;
    if (*double_a < *double_b) return FALSE;
    return TRUE;
}
boolean stringCompare(const void *a, const void *b) {
    if (a == NULL || b == NULL) return FALSE;
    char *string_a = (char *)a;
    char *string_b = (char *)b;
    return strcmp(string_a, string_b) <= 0 ? FALSE : TRUE;
}
void * doubleAllocate() {
    return malloc(sizeof(double));
}
void * stringAllocate() {
    return malloc(STRING_SIZE);//возможно тут лучше по другому
}
void stringDeallocate(void *ptr) {
    free(ptr);
}
void doubleDeallocate(void *ptr) {
    free(ptr);
}
void doubleAssign(void *res, void *arg) {
    memcpy(res, arg, sizeof(double));
}
void stringAssign(void *res, void *arg) {
    memcpy(res, arg, STRING_SIZE);
}
const FieldInfo * getDoubleFieldInfo() {
    if (DOUBLE_FIELD_INFO == NULL) {
        DOUBLE_FIELD_INFO = (FieldInfo *)malloc(sizeof(FieldInfo));
        DOUBLE_FIELD_INFO->size = PTR_SIZE;
        DOUBLE_FIELD_INFO->name = "double";
        DOUBLE_FIELD_INFO->compare = doubleCompare;
        DOUBLE_FIELD_INFO->allocate = doubleAllocate;
        DOUBLE_FIELD_INFO->deallocate = doubleDeallocate;
        DOUBLE_FIELD_INFO->assign = doubleAssign;
    }
    return DOUBLE_FIELD_INFO;
}
const FieldInfo * getStringFieldInfo() {
    if (STRING_FIELD_INFO == NULL) {
        STRING_FIELD_INFO = (FieldInfo *)malloc(sizeof(FieldInfo));
        STRING_FIELD_INFO->size = PTR_SIZE;
        STRING_FIELD_INFO->name = "string";
        STRING_FIELD_INFO->compare = stringCompare;
        STRING_FIELD_INFO->allocate = stringAllocate;
        STRING_FIELD_INFO->deallocate = stringDeallocate;
        STRING_FIELD_INFO->assign = stringAssign;
    }
    return STRING_FIELD_INFO;
}

