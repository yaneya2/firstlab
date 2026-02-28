#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/FieldInfo.h"
#include "../headers/DynamicArray.h"
#include  "../headers/UsersStruct.h"
// Внешние переменные для хранения текущего списка и типа данных
static DynamicArray *current_dynamicArray = NULL;

// Функция для инициализации типов данных (предполагается, что они определены где-то в других файлах)
extern const FieldInfo * getDoubleFieldInfo();
extern const FieldInfo * getStringFieldInfo();
extern const FieldInfo * getPointFieldInfo();

// Функция для очистки списка
void clearDynamicArray() {
    deleteDynamicArray(current_dynamicArray);
}
// Безопасное чтение целого числа с очисткой буфера
int readInt(const char *prompt) {
    int value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) == 1) {
            // Очищаем буфер до конца строки
            while (getchar() != '\n');
            return value;
        } else {
            printf("Invalid input. Please enter an integer.\n\n");
            // Очищаем некорректный ввод
            while (getchar() != '\n');
        }
    }
}

// Безопасное чтение вещественного числа
double readDouble(const char *prompt) {
    double value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%lf", &value) == 1) {
            while (getchar() != '\n');
            return value;
        } else {
            printf("Invalid input. Please enter a real number.\n\n");
            while (getchar() != '\n');
        }
    }
}
// Вспомогательные функции для проверки map и where

// Проверочная функция для where для double (фильтр: положительные числа)
bool positiveFilterFunction(const void *element) {
    double value = *(double*)element;
    return value > 0;
}

// Проверочная функция для map для double (преобразование: удвоение значения)
void* doubleValueFunction(void *element) {
    double* original_value = (double*)element;
    *original_value *=2;
    return original_value;
}

// Проверочная функция для where для строк (фильтр: длина строки > 3)
bool stringLengthFilterFunction(const void *element) {
    char *str = (char*)element;
    return str != NULL && strlen(str) > 3;
}

// Проверочная функция для map для строк (преобразование: в верхний регистр)
void* toUppercaseFunction(void *element) {
    char *original_str = (char*)element;
    if(original_str == NULL) return NULL;
    
    int len = strlen(original_str);
    char *upper_str = malloc((len + 1) * sizeof(char));
    if(upper_str != NULL) {
        for(int i = 0; i <= len; i++) {
            if(original_str[i] >= 'a' && original_str[i] <= 'z') {
                upper_str[i] = original_str[i] - 'a' + 'A';
            } else {
                upper_str[i] = original_str[i];
            }
        }
        upper_str[len] = '\0';
    }
    current_dynamicArray->field_info->deallocate(original_str);
    // заменяем строку , если работать со старой, то можно вернуть ее же
    return upper_str;
}

// Проверочная функция для where для Point (фильтр: точки с положительными координатами)
bool positivePointFilterFunction(const void *element) {
    Point *p = (Point*)element;
    return p != NULL && p->x > 0 && p->y > 0 && p->z > 0;
}

// Проверочная функция для map для Point (преобразование: увеличение координат на 1)
void* incrementPointFunction(void *element) {
    Point *point = (Point*)element;
    point->x += 1;
    point->y += 1;
    point->z += 1;
    return point;
}

void doubleUI() {
    int choice;
    double value;
    int index;

    while(1) {
        printf("\n=== Double DynamicArray Menu ===\n");
        printf("1. Create dynamicArray\n");
        printf("2. Add element\n");
        printf("3. Get element\n");
        printf("4. Sort dynamicArray\n");
        printf("5. Concatenate dynamicArray with itself\n");
        printf("6. Apply where\n");
        printf("7. Apply map\n");
        printf("8. Print all elements\n");
        printf("9. Delete dynamicArray and all elements\n");
        printf("0. Exit\n");

        choice = readInt("Enter your choice: ");

        switch(choice) {
            case 1:
                if (current_dynamicArray != NULL) {
                    printf("Warning: Current dynamicArray will be deleted. ");
                    clearDynamicArray();
                }
                current_dynamicArray = createEmptyDynamicArray(getDoubleFieldInfo());
                printf("Double dynamicArray created successfully.\n");
                break;

            case 2:
                if (current_dynamicArray == NULL) {
                    printf("Error: No dynamicArray created. Create a dynamicArray first.\n");
                    break;
                }
                value = readDouble("Enter double value to add: ");
                add(current_dynamicArray, &value);
                printf("Element added successfully.\n");
                break;

            case 3:
                if (current_dynamicArray == NULL) {
                    printf("Error: No dynamicArray created.\n");
                    break;
                }
                index = readInt("Enter index to get element: ");
                if (index >= 0 && index < current_dynamicArray->size) {
                    double* element = (double*)get(current_dynamicArray, index);
                    if (element != NULL) {
                        printf("Element at index %d: %f\n", index, *element);
                    } else {
                        printf("Error getting element.\n");
                    }
                } else {
                    printf("Invalid index. Please enter a number between 0 and %d.\n", current_dynamicArray->size - 1);
                }
                break;

            case 4:
                if (current_dynamicArray == NULL) {
                    printf("Error: No dynamicArray created.\n");
                    break;
                }
                sort(current_dynamicArray);
                printf("DynamicArray sorted successfully.\n");
                break;

            case 5:
                if (current_dynamicArray == NULL) {
                    printf("Error: No dynamicArray created.\n");
                    break;
                }
                {
                    DynamicArray* temp = concat(current_dynamicArray, current_dynamicArray);
                    if (temp != NULL) {
                        deleteDynamicArray(current_dynamicArray);
                        current_dynamicArray = temp;
                        printf("DynamicArray concatenated with itself successfully.\n");
                    } else {
                        printf("Error concatenating dynamicArrays.\n");
                    }
                }
                break;

            case 6:
                if (current_dynamicArray == NULL) {
                    printf("Error: No dynamicArray created.\n");
                    break;
                }
                printf("Applying where filter (positive numbers only)\n");
                where(current_dynamicArray, positiveFilterFunction);
                break;

            case 7:
                if (current_dynamicArray == NULL) {
                    printf("Error: No dynamicArray created.\n");
                    break;
                }
                printf("Applying map (doubling values)\n");
                map(current_dynamicArray, doubleValueFunction);
                break;

            case 8:
                if (current_dynamicArray == NULL) {
                    printf("Error: No dynamicArray created.\n");
                    break;
                }
                printf("DynamicArray elements: ");
                for (int i = 0; i < current_dynamicArray->size; i++) {
                    double *element = (double*)get(current_dynamicArray, i);
                    if (element != NULL) {
                        printf("%.2f ", *element);
                    }
                }
                printf("\n");
                break;

            case 9:
                clearDynamicArray();
                current_dynamicArray = NULL;
                printf("DynamicArray and all elements deleted.\n");
                break;

            case 0:
                if (current_dynamicArray != NULL) {
                    clearDynamicArray();
                }
                printf("Exiting double UI...\n");
                return;

            default:
                printf("Invalid choice. Please enter a number from the menu options.\n");
                break;
        }
    }
}

void pointUI() {
    int choice;
    Point value;
    int index;

    while(1) {
        printf("\n=== Point DynamicArray Menu ===\n");
        printf("1. Create dynamicArray\n");
        printf("2. Add element\n");
        printf("3. Get element\n");
        printf("4. Sort dynamicArray\n");
        printf("5. Concatenate dynamicArray with itself\n");
        printf("6. Apply where\n");
        printf("7. Apply map\n");
        printf("8. Print all elements\n");
        printf("9. Delete dynamicArray and all elements\n");
        printf("0. Exit\n");

        choice = readInt("Enter your choice: ");

        switch(choice) {
            case 1:
                if (current_dynamicArray != NULL) {
                    printf("Warning: Current dynamicArray will be deleted. ");
                    clearDynamicArray();
                }
                current_dynamicArray = createEmptyDynamicArray(getPointFieldInfo());
                printf("Point dynamicArray created successfully.\n");
                break;

            case 2:
                if (current_dynamicArray == NULL) {
                    printf("Error: No dynamicArray created. Create a dynamicArray first.\n");
                    break;
                }
                value.x = readDouble("Enter x coordinate: ");
                value.y = readDouble("Enter y coordinate: ");
                value.z = readDouble("Enter z coordinate: ");
                add(current_dynamicArray, &value);
                printf("Element added successfully.\n");
                break;

            case 3:
                if (current_dynamicArray == NULL) {
                    printf("Error: No dynamicArray created.\n");
                    break;
                }
                index = readInt("Enter index to get element: ");
                if (index >= 0 && index < current_dynamicArray->size) {
                    Point* element = (Point*)get(current_dynamicArray, index);
                    if (element != NULL) {
                        printf("Element at index %d: (%.2f, %.2f, %.2f)\n", index, element->x, element->y, element->z);
                    } else {
                        printf("Error getting element.\n");
                    }
                } else {
                    printf("Invalid index. Please enter a number between 0 and %d.\n", current_dynamicArray->size - 1);
                }
                break;

            case 4:
                if (current_dynamicArray == NULL) {
                    printf("Error: No dynamicArray created.\n");
                    break;
                }
                sort(current_dynamicArray);
                printf("DynamicArray sorted successfully.\n");
                break;

            case 5:
                if (current_dynamicArray == NULL) {
                    printf("Error: No dynamicArray created.\n");
                    break;
                }
                {
                    DynamicArray* temp = concat(current_dynamicArray, current_dynamicArray);
                    if (temp != NULL) {
                        deleteDynamicArray(current_dynamicArray);
                        current_dynamicArray = temp;
                        printf("DynamicArray concatenated with itself successfully.\n");
                    } else {
                        printf("Error concatenating dynamicArrays.\n");
                    }
                }
                break;

            case 6:
                if (current_dynamicArray == NULL) {
                    printf("Error: No dynamicArray created.\n");
                    break;
                }
                printf("Applying where filter (points with positive coordinates only)\n");
                where(current_dynamicArray, positivePointFilterFunction);
                break;

            case 7:
                if (current_dynamicArray == NULL) {
                    printf("Error: No dynamicArray created.\n");
                    break;
                }
                printf("Applying map (incrementing coordinates by 1)\n");
                map(current_dynamicArray, incrementPointFunction);
                break;

            case 8:
                if (current_dynamicArray == NULL) {
                    printf("Error: No dynamicArray created.\n");
                    break;
                }
                printf("DynamicArray elements: ");
                for (int i = 0; i < current_dynamicArray->size; i++) {
                    Point *element = (Point*)get(current_dynamicArray, i);
                    if (element != NULL) {
                        printf("(%.2f, %.2f, %.2f) ", element->x, element->y, element->z);
                    }
                }
                printf("\n");
                break;

            case 9:
                clearDynamicArray();
                current_dynamicArray = NULL;
                printf("DynamicArray and all elements deleted.\n");
                break;

            case 0:
                if (current_dynamicArray != NULL) {
                    clearDynamicArray();
                }
                printf("Exiting point UI...\n");
                return;

            default:
                printf("Invalid choice. Please enter a number from the menu options.\n");
                break;
        }
    }
}

void stringUI() {
    int choice;
    char buffer[256];
    int index;

    while(1) {
        printf("\n=== String DynamicArray Menu ===\n");
        printf("1. Create dynamicArray\n");
        printf("2. Add element\n");
        printf("3. Get element\n");
        printf("4. Sort dynamicArray\n");
        printf("5. Concatenate dynamicArray with itself\n");
        printf("6. Apply where\n");
        printf("7. Apply map\n");
        printf("8. Print all elements\n");
        printf("9. Delete dynamicArray and all elements\n");
        printf("0. Exit\n");

        choice = readInt("Enter your choice: ");

        switch(choice) {
            case 1:
                if (current_dynamicArray != NULL) {
                    printf("Warning: Current dynamicArray will be deleted. ");
                    clearDynamicArray();
                }
                current_dynamicArray = createEmptyDynamicArray(getStringFieldInfo());
                printf("String dynamicArray created successfully.\n");
                break;

            case 2:
                if (current_dynamicArray == NULL) {
                    printf("Error: No dynamicArray created. Create a dynamicArray first.\n");
                    break;
                }
                printf("Enter string to add: ");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = 0;
                add(current_dynamicArray, buffer);
                printf("Element added successfully.\n");
                break;

            case 3:
                if (current_dynamicArray == NULL) {
                    printf("Error: No dynamicArray created.\n");
                    break;
                }
                index = readInt("Enter index to get element: ");
                if (index >= 0 && index < current_dynamicArray->size) {
                    char *element = (char*)get(current_dynamicArray, index);
                    if (element != NULL) {
                        printf("Element at index %d: %s\n", index, element);
                    } else {
                        printf("Error getting element.\n");
                    }
                } else {
                    printf("Invalid index. Please enter a number between 0 and %d.\n", current_dynamicArray->size - 1);
                }
                break;

            case 4:
                if (current_dynamicArray == NULL) {
                    printf("Error: No dynamicArray created.\n");
                    break;
                }
                sort(current_dynamicArray);
                printf("DynamicArray sorted successfully.\n");
                break;

            case 5:
                if (current_dynamicArray == NULL) {
                    printf("Error: No dynamicArray created.\n");
                    break;
                }
                {
                    DynamicArray *temp = concat(current_dynamicArray, current_dynamicArray);
                    deleteDynamicArray(current_dynamicArray);
                    current_dynamicArray = temp;
                    printf("DynamicArray concatenated with itself successfully.\n");
                }
                break;

            case 6:
                if (current_dynamicArray == NULL) {
                    printf("Error: No dynamicArray created.\n");
                    break;
                }
                printf("Applying where filter (strings longer than 3 chars)\n");
                where(current_dynamicArray, stringLengthFilterFunction);
                break;

            case 7:
                if (current_dynamicArray == NULL) {
                    printf("Error: No dynamicArray created.\n");
                    break;
                }
                printf("Applying map (converting to uppercase)\n");
                map(current_dynamicArray, toUppercaseFunction);
                break;

            case 8:
                if (current_dynamicArray == NULL) {
                    printf("Error: No dynamicArray created.\n");
                    break;
                }
                printf("DynamicArray elements: [");
                for (int i = 0; i < current_dynamicArray->size; i++) {
                    char *element = (char*)get(current_dynamicArray, i);
                    if (element != NULL) {
                        printf("\"%s\"", element);
                        if (i < current_dynamicArray->size - 1) printf(", ");
                    }
                }
                printf("]\n");
                break;

            case 9:
                clearDynamicArray();
                current_dynamicArray = NULL;
                printf("DynamicArray and all elements deleted.\n");
                break;

            case 0:
                printf("Exiting string UI...\n");
                return;

            default:
                printf("Invalid choice. Please enter a number from the menu options.\n");
                break;
        }
    }
}