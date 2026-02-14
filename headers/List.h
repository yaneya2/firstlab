//
// Created by Vasya on 07.02.2026.
//

#ifndef FIRSTLAB_LIST_H
#define FIRSTLAB_LIST_H
#include <stdio.h>
#include <windows.h>

#include "FieldInfo.h"
#define CAPACITY 10
typedef struct {
    int capacity;
    int size;
    FieldInfo *field_info;
    void * data;
}List;
List * createList(int capacity, const FieldInfo *field_info);
List * createEmptyList(const FieldInfo *field_info);
void printList(const List *list);
void add(List * list,const void * ell);
void * get(const List * list, int index);
void set(List * list, int index, const void * ell);
void map(List *list, void * (*function)(void *));
void where(List *list, boolean (*function)(void *));
List * concat(const List *list1, const List *list2);
void sort(List *list);

#endif //FIRSTLAB_LIST_H