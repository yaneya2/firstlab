#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include "../headers/FieldInfo.h"
#include "../headers/UsersStruct.h"

#define STRING_SIZE 50

static FieldInfo *DOUBLE_FIELD_INFO = NULL;
static FieldInfo *STRING_FIELD_INFO = NULL;
static FieldInfo *POINT_FIELD_INFO = NULL;


static int doubleCompare(const void *a, const void *b) {
    double *double_a = (double *) a;
    double *double_b = (double *) b;

    if (*double_a < *double_b) return -1;
    if (*double_a > *double_b) return 1;

    return 0;
}

static int stringCompare(const void *a, const void *b) {
    char *string_a = (char *) a;
    char *string_b = (char *) b;

    return strcmp(string_a, string_b);
}

static int PointCompare(const void *a, const void *b) {
    Point *point_a = (Point *) a;
    Point *point_b = (Point *) b;

    double dist_a = point_a->x * point_a->x + point_a->y * point_a->y;
    double dist_b = point_b->x * point_b->x + point_b->y * point_b->y;

    if (dist_a < dist_b) return -1;
    if (dist_a > dist_b) return 1;

    return 0;
}


static bool doubleAllocate(void *ptr) {
    if (ptr == NULL) {
        errno = EINVAL;
        return false;
    }

    return true;
}

static bool stringAllocate(void *ptr) {
    if (ptr == NULL) {
        errno = EINVAL;
        return false;
    }

    return true;
}

static bool pointAllocate(void *ptr) {
    if (ptr == NULL) {
        errno = EINVAL;
        return false;
    }

    return true;
}


static bool stringDeallocate(void *ptr) {
    if (ptr == NULL) {
        errno = EINVAL;
        return false;
    }

    return true;
}

static bool doubleDeallocate(void *ptr) {
    if (ptr == NULL) {
        errno = EINVAL;
        return false;
    }

    return true;
}

static bool pointDeallocate(void *ptr) {
    if (ptr == NULL) {
        errno = EINVAL;
        return false;
    }

    return true;
}


static bool doubleAssign(void *res, const void *arg) {
    if (res == NULL || arg == NULL) {
        errno = EINVAL;
        return false;
    }

    double *double_res = (double *) res;
    double *double_arg = (double *) arg;
    *double_res = *double_arg;

    return true;
}

static bool stringAssign(void *res, const void *arg) {
    if (res == NULL || arg == NULL) {
        errno = EINVAL;
        return false;
    }

    char *string_arg = (char *) arg;
    int string_arg_len = strlen(string_arg);

    if (string_arg_len > STRING_SIZE) {
        errno = EINVAL;
        return false;
    }

    strncpy((char *) res, (const char *) arg, STRING_SIZE - 1);
    ((char *) res)[STRING_SIZE - 1] = '\0';

    return true;
}

static bool pointAssign(void *res, const void *arg) {
    if (res == NULL || arg == NULL) {
        errno = EINVAL;
        return false;
    }

    Point *point_res = (Point *) res;
    Point *point_arg = (Point *) arg;
    *point_res = *point_arg;

    return true;
}


const FieldInfo *getDoubleFieldInfo() {
    if (DOUBLE_FIELD_INFO == NULL) {
        DOUBLE_FIELD_INFO = (FieldInfo *) malloc(sizeof(FieldInfo));

        if (DOUBLE_FIELD_INFO == NULL) {
            errno = ENOMEM;
            return NULL;
        }

        DOUBLE_FIELD_INFO->size = sizeof(double);
        DOUBLE_FIELD_INFO->name = "double";
        DOUBLE_FIELD_INFO->compare = doubleCompare;
        DOUBLE_FIELD_INFO->allocate = doubleAllocate;
        DOUBLE_FIELD_INFO->deallocate = doubleDeallocate;
        DOUBLE_FIELD_INFO->assign = doubleAssign;
    }

    return DOUBLE_FIELD_INFO;
}

const FieldInfo *getStringFieldInfo() {
    if (STRING_FIELD_INFO == NULL) {
        STRING_FIELD_INFO = (FieldInfo *) malloc(sizeof(FieldInfo));

        if (STRING_FIELD_INFO == NULL) {
            errno = ENOMEM;
            return NULL;
        }

        STRING_FIELD_INFO->size = STRING_SIZE;
        STRING_FIELD_INFO->name = "string";
        STRING_FIELD_INFO->compare = stringCompare;
        STRING_FIELD_INFO->allocate = stringAllocate;
        STRING_FIELD_INFO->deallocate = stringDeallocate;
        STRING_FIELD_INFO->assign = stringAssign;
    }

    return STRING_FIELD_INFO;
}

const FieldInfo *getPointFieldInfo() {
    if (POINT_FIELD_INFO == NULL) {
        POINT_FIELD_INFO = (FieldInfo *) malloc(sizeof(FieldInfo));

        if (POINT_FIELD_INFO == NULL) {
            errno = ENOMEM;
            return NULL;
        }

        POINT_FIELD_INFO->size = sizeof(Point);
        POINT_FIELD_INFO->name = "point";
        POINT_FIELD_INFO->compare = PointCompare;
        POINT_FIELD_INFO->allocate = pointAllocate;
        POINT_FIELD_INFO->deallocate = pointDeallocate;
        POINT_FIELD_INFO->assign = pointAssign;
    }

    return POINT_FIELD_INFO;
}