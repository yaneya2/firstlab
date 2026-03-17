#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include "../headers/FieldInfo.h"
#include "../headers/UsersStruct.h"

#define STRING_SIZE 50

static FieldInfo *double_field_info = NULL;
static FieldInfo *string_field_info = NULL;
static FieldInfo *point_field_info = NULL;


static int double_compare(const void *a, const void *b) {
    double *double_a = (double *) a;
    double *double_b = (double *) b;

    if (*double_a < *double_b) return -1;
    if (*double_a > *double_b) return 1;

    return 0;
}

static int string_compare(const void *a, const void *b) {
    char *string_a = (char *) a;
    char *string_b = (char *) b;

    return strcmp(string_a, string_b);
}

static int point_compare(const void *a, const void *b) {
    Point *point_a = (Point *) a;
    Point *point_b = (Point *) b;

    double dist_a = point_a->x * point_a->x + point_a->y * point_a->y;
    double dist_b = point_b->x * point_b->x + point_b->y * point_b->y;

    if (dist_a < dist_b) return -1;
    if (dist_a > dist_b) return 1;

    return 0;
}


static bool double_allocate(void *ptr) {
    if (ptr == NULL) {
        errno = EINVAL;
        return false;
    }

    return true;
}

static bool string_allocate(void *ptr) {
    if (ptr == NULL) {
        errno = EINVAL;
        return false;
    }

    return true;
}

static bool point_allocate(void *ptr) {
    if (ptr == NULL) {
        errno = EINVAL;
        return false;
    }

    return true;
}


static bool string_deallocate(void *ptr) {
    if (ptr == NULL) {
        errno = EINVAL;
        return false;
    }

    return true;
}

static bool double_deallocate(void *ptr) {
    if (ptr == NULL) {
        errno = EINVAL;
        return false;
    }

    return true;
}

static bool point_deallocate(void *ptr) {
    if (ptr == NULL) {
        errno = EINVAL;
        return false;
    }

    return true;
}


static bool double_assign(void *res, const void *arg) {
    if (res == NULL || arg == NULL) {
        errno = EINVAL;
        return false;
    }

    double *double_res = (double *) res;
    double *double_arg = (double *) arg;
    *double_res = *double_arg;

    return true;
}

static bool string_assign(void *res, const void *arg) {
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

static bool point_assign(void *res, const void *arg) {
    if (res == NULL || arg == NULL) {
        errno = EINVAL;
        return false;
    }

    Point *point_res = (Point *) res;
    Point *point_arg = (Point *) arg;
    *point_res = *point_arg;

    return true;
}


const FieldInfo *get_double_field_info() {
    if (double_field_info == NULL) {
        double_field_info = (FieldInfo *) malloc(sizeof(FieldInfo));

        if (double_field_info == NULL) {
            errno = ENOMEM;
            return NULL;
        }

        double_field_info->size = sizeof(double);
        double_field_info->name = "double";
        double_field_info->compare = double_compare;
        double_field_info->allocate = double_allocate;
        double_field_info->deallocate = double_deallocate;
        double_field_info->assign = double_assign;
    }

    return double_field_info;
}

const FieldInfo *get_string_field_info() {
    if (string_field_info == NULL) {
        string_field_info = (FieldInfo *) malloc(sizeof(FieldInfo));

        if (string_field_info == NULL) {
            errno = ENOMEM;
            return NULL;
        }

        string_field_info->size = STRING_SIZE;
        string_field_info->name = "string";
        string_field_info->compare = string_compare;
        string_field_info->allocate = string_allocate;
        string_field_info->deallocate = string_deallocate;
        string_field_info->assign = string_assign;
    }

    return string_field_info;
}

const FieldInfo *get_point_field_info() {
    if (point_field_info == NULL) {
        point_field_info = (FieldInfo *) malloc(sizeof(FieldInfo));

        if (point_field_info == NULL) {
            errno = ENOMEM;
            return NULL;
        }

        point_field_info->size = sizeof(Point);
        point_field_info->name = "point";
        point_field_info->compare = point_compare;
        point_field_info->allocate = point_allocate;
        point_field_info->deallocate = point_deallocate;
        point_field_info->assign = point_assign;
    }

    return point_field_info;
}