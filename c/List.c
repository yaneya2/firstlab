//
// Created by Vasya on 07.02.2026.
//
#include "../headers/List.h"

List * createList(int capacity, const FieldInfo *field_info) {
    if (field_info == NULL || capacity <= 0) return NULL;
    List * list = (List*)malloc(sizeof(List));
    list->capacity = capacity;
    list->size = 0;
    list->field_info = field_info;
    list->data = malloc(list->capacity * PTR_SIZE);
    return list;
}
List * createEmptyList(const FieldInfo *field_info) {
    if (field_info == NULL) return NULL;
    List * list = (List*)malloc(sizeof(List));
    list->capacity = CAPACITY;
    list->size = 0;
    list->field_info = field_info;
    list->data = malloc(list->capacity * PTR_SIZE);
    return list;
}
void add(List *list,const void *ell) {
    if (list == NULL || ell == NULL) return;
    if (list->size == list->capacity) {
        list->capacity *= 2;
        list->data = realloc(list->data, list->capacity * PTR_SIZE);
    }
    set(list, list->size++, ell);
}
void * get(const List *list, int index) {
    if (list == NULL || index < 0 || index >= list->size) return NULL;
    return list->data[index];
}
static void set(List *list, int index, const void *ell) {//служебный метод
    if (list == NULL || index < 0 || index >= list->size) return;
    list->data[index] = list->field_info->allocate();
    list->field_info->assign(list->data[index], ell);
}

void map(List * list, void *(*function)(void *)) {
    for (int i = 0; i < list->size; ++i) {
        set(list, i,function(get(list, i)));
    }
}

void where(List *list, boolean (*function)(const void *)) {
    for (int i = 0; i < list->size; ++i) {
        if (!function(get(list, i))) {
            list->field_info->deallocate(get(list, list->size));
            for (int j = i; j < list->size - 1; ++j) {
                set(list, j, get(list, j + 1));
            }
            list->field_info->deallocate(get(list, list->size));
            list->size--;
            i--;
        }
    }
}
List * concat(const List *list1, const List *list2) {
    if (list1 == NULL || list2 == NULL) return NULL;
    if (list1->field_info != list2->field_info) return NULL;
    List * list = createList(list1->capacity + list2->capacity, list1->field_info);
    for (int i = 0; i < list1->size; ++i) {
        add(list, get(list1, i));
    }
    for (int i = 0; i < list2->size; ++i) {
        add(list, get(list2, i));
    }
    return list;
}
void sort(List *list) {
    if (list == NULL || list->size == 0) return;
    int minIndex = 0;
    void *t = 0;
    for (int i = 0; i < list->size - 1; i++) {
        minIndex = i;
        for (int j = i + 1; j < list->size; j++) {
            if (!list->field_info->compare(get(list, j), get(list, minIndex))){
                minIndex = j;
            }
        }
        t = get(list, i);
        set(list, i, get(list, minIndex));
        set(list, minIndex, t);
    }
}
void deleteList(List *list) {
    if (list == NULL) return;
    for (int i = 0; i < list->size; i++) {
        list->field_info->deallocate(get(list, i));
    }
    free(list->data);
    free(list);
}
