#include <stdio.h>

#include "headers/List.h"
#include "headers/FieldInfo.h"
#include <math.h>
//26 вариант
// void * forDoubleMap(void *);
// boolean forWhere(void *);
// void * forStringMap(void *);
int main(void) {
    // List * list = createEmptyList(getStringFieldInfo());
    // char *a = "b", *b = "c", *c = "a", *d = "d";
    // add(list,a);
    // add(list,b);
    // add(list,c);
    // add(list,d);
    List *list = createEmptyList(getDoubleFieldInfo());
     double a = 12, b = -20, c = 30;
     double *p_a = &a, *p_b = &b, *p_c = &c;
     add(list, p_a);
     add(list, p_b);
     add(list, p_c);
    sort(list);
    // // map(list, forDoubleMap);
    printList(list);
}
//void * forDoubleMap(void * x) {
//     double * t = (double *)x;
//     *t = sin(*t);
//     return (void *)t;
// }
// void * forStringMap(void * x) {
//     char t[50] = "123";
//     char * s = strcat(t,(char *)x);
//     return s;
// }
// boolean forWhere(void * x) {
//     double * t = (double *)x;
//     if (*t > 26) {
//         return TRUE;
//     }
//     return FALSE;
// }
