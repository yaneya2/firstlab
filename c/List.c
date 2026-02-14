//
// Created by Vasya on 07.02.2026.
//
#include "../headers/List.h"
List * createEmptyList(const FieldInfo *field_info) {
    if (field_info == NULL) return NULL;
    List * list = (List*)malloc(sizeof(List));
    list->capacity = CAPACITY;
    list->size = 0;
    list->field_info = field_info;
    list->data = malloc(list->capacity * PTR_SIZE);
    return list;
}

void * get(const List *list, int index) {
    if (list == NULL || index < 0 || index >= list->size) return NULL;
    void ** t = (void **)list->data;
    return t[index];
}

void set(List *list, int index, const void *ell) {
    if (list == NULL || index < 0 || index >= list->size) return;
    void ** t = (void **)list->data;
    t[index] = ell;
}

void add(List *list,const void *ell) {
    if (list == NULL || ell == NULL) return;
    if (list->size == list->capacity) {
        list->capacity *= 2;
        list->data = realloc(list->data, list->capacity * PTR_SIZE);
    }
    set(list, list->size++, ell);
}

/*
void printList(const List *list) {
    if (list->size == 0 || list == NULL) return;
    for (int i = 0; i < list->size; i++) {
        printf("%s\n", list->field_info->toString(get(list, i)));
    }
}
*/

void sort(List *list) {
    if (list->size == 0 || list == NULL) return;
    int minIndex = 0;
    void * temp = 0;
    for (int i = 0; i < list->size - 1; i++) {
        minIndex = i;
        for (int j = i + 1; j < list->size; j++) {
            if (!list->field_info->compare(get(list, j), get(list, minIndex))){
                minIndex = j;
            }
        }
        set(list, i, get(list, minIndex));
        set(list, minIndex, temp);
        //printList(list);
        printf("\n");
    }
}

List * createList(int capacity, const FieldInfo *field_info) {
    List * list = (List*)malloc(sizeof(List));
    list->capacity = capacity;
    list->size = PTR_SIZE;
    list->field_info = field_info;
    list->data = malloc(list->capacity * PTR_SIZE);
    return list;
}
void map(List * list, void *(*function)(void *)) {
    for (int i = 0; i < list->size; ++i) {
        set(list, i,function(get(list, i)));
    }
}

void where(List *list, int (*function)(void *)) {
    for (int i = 0; i < list->size; ++i) {
        if (!function(get(list, i))) {
            for (int j = i; j < list->size - 1; ++j) {
                set(list, j, get(list, j + 1));
            }
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