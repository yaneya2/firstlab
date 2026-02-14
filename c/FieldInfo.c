#include <stddef.h>
#include "../headers/FieldInfo.h"

#include <stdlib.h>
//
// Created by Vasya on 07.02.2026.
//
#include <stdio.h>
#include <windows.h>
FieldInfo *DOUBLE_FIELD_INFO = NULL;
FieldInfo *STRING_FIELD_INFO = NULL;

boolean compareDouble(const void * a, const void * b) {
    if (a == NULL || b == NULL) return FALSE;
    double * double_a = (double *) a;
    double * double_b = (double *) b;
    if (*double_a < *double_b) return FALSE;
    return TRUE;
}
boolean compareString(const void * a, const void * b) {
    if (a == NULL || b == NULL) return FALSE;
    char *string_a = (char *) a;
    char *string_b = (char *) b;
    return strcmp(string_a, string_b) <= 0 ? FALSE : TRUE;
}
char * doubleToString(const void * p) {
    if (p == NULL) return NULL;
    char * res = (char *)malloc(50);
    sprintf(res, "%.3lf", *((double *) p));
    return res;
}

char * charToString(const void * p) {
    return (char *)p;
}

const FieldInfo * getDoubleFieldInfo() {
    if (DOUBLE_FIELD_INFO == NULL) {
        DOUBLE_FIELD_INFO = (FieldInfo *)malloc(sizeof(FieldInfo));
        DOUBLE_FIELD_INFO->size = PTR_SIZE;
        DOUBLE_FIELD_INFO->name = "double";
        DOUBLE_FIELD_INFO->compare = compareDouble;
        DOUBLE_FIELD_INFO->toString = doubleToString;
    }
    return DOUBLE_FIELD_INFO;
}

const FieldInfo * getStringFieldInfo() {
    if (STRING_FIELD_INFO == NULL) {
        STRING_FIELD_INFO = (FieldInfo *)malloc(sizeof(FieldInfo));
        STRING_FIELD_INFO->size = PTR_SIZE;
        STRING_FIELD_INFO->name = "string";
        STRING_FIELD_INFO->compare = compareString;
        STRING_FIELD_INFO->toString = charToString;
    }
    return STRING_FIELD_INFO;
}

