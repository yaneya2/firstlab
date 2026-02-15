#include <stdio.h>
#include <stdlib.h>
#include "../headers/List.h"
#include "../headers/FieldInfo.h"

// Внешние переменные для хранения текущего списка и типа данных
static List* current_list = NULL;

// Функция для инициализации типов данных (предполагается, что они определены где-то в других файлах)
extern const FieldInfo* getDoubleFieldInfo();
extern const FieldInfo* getStringFieldInfo();

// Функция для очистки списка
void clearList() {
    if (current_list != NULL) {
        // Освобождаем каждый элемент списка
        for (int i = 0; i < current_list->size; i++) {
            current_list->field_info->deallocate(get(current_list, i));
        }
        // Освобождаем сам список
        free(current_list->data);
        free(current_list);
        current_list = NULL;
    }
}

// Вспомогательные функции для проверки map и where

// Проверочная функция для where для double (фильтр: положительные числа)
boolean positiveFilterFunction(const void* element) {
    double value = *(double*)element;
    return value > 0;
}

// Проверочная функция для map для double (преобразование: удвоение значения)
void* doubleValueFunction(void* element) {
    double* original_value = (double*)element;
    *original_value *=2;
    return original_value;
}

// Проверочная функция для where для строк (фильтр: длина строки > 3)
boolean stringLengthFilterFunction(const void* element) {
    char* str = (char*)element;
    return str != NULL && strlen(str) > 3;
}

// Проверочная функция для map для строк (преобразование: в верхний регистр)
void* toUppercaseFunction(void* element) {
    char* original_str = (char*)element;
    if(original_str == NULL) return NULL;
    
    int len = strlen(original_str);
    char* upper_str = malloc((len + 1) * sizeof(char));
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
    current_list->field_info->deallocate(original_str);
    // заменяем строку , если работать со старой, то можно вернуть ее же
    return upper_str;
}

// Консольный интерфейс для работы с double
void doubleUI() {
    int choice;
    double value;
    int index;
    
    while(1) {
        printf("\n=== Double List Menu ===\n");
        printf("1. Create list\n");
        printf("2. Add element\n");
        printf("3. Get element\n");
        printf("4. Sort list\n");
        printf("5. Concatenate list with itself\n");
        printf("6. Apply where\n");
        printf("7. Apply map\n");
        printf("8. Print all elements\n");
        printf("9. Delete list and all elements\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: // Создание листа
                if (current_list != NULL) {
                    printf("Warning: Current list will be deleted. ");
                    clearList();
                }
                current_list = createEmptyList(getDoubleFieldInfo());
                printf("Double list created successfully.\n");
                break;
                
            case 2: // Добавление элемента
                if (current_list == NULL) {
                    printf("Error: No list created. Create a list first.\n");
                    break;
                }
                printf("Enter double value to add: ");
                scanf("%lf", &value);
                add(current_list, &value);
                printf("Element added successfully.\n");
                break;
                
            case 3: // Получение элемента
                if (current_list == NULL) {
                    printf("Error: No list created.\n");
                    break;
                }
                printf("Enter index to get element: ");
                scanf("%d", &index);
                if (index >= 0 && index < current_list->size) {
                    double* element = (double*)get(current_list, index);
                    if (element != NULL) {
                        printf("Element at index %d: %f\n", index, *element);
                    } else {
                        printf("Error getting element.\n");
                    }
                } else {
                    printf("Invalid index.\n");
                }
                break;
                
            case 4: // Сортировка листа
                if (current_list == NULL) {
                    printf("Error: No list created.\n");
                    break;
                }
                sort(current_list);
                printf("List sorted successfully.\n");
                break;
                
            case 5: // Конкатенация листа с самим собой
                if (current_list == NULL) {
                    printf("Error: No list created.\n");
                    break;
                }
                {
                    List* temp = concat(current_list, current_list);
                    if (temp != NULL) {
                        // Освобождаем старый список
                        for (int i = 0; i < current_list->size; i++) {
                            current_list->field_info->deallocate(current_list->data[i]);
                        }
                        free(current_list->data);
                        free(current_list);
                        
                        // Присваиваем новый конкатенированный список
                        current_list = temp;
                        printf("List concatenated with itself successfully.\n");
                    } else {
                        printf("Error concatenating lists.\n");
                    }
                }
                break;
                
            case 6: // Применение where
                if (current_list == NULL) {
                    printf("Error: No list created.\n");
                    break;
                }
                // Предполагаем, что есть функция для фильтрации, например, больше 0
                printf("Applying where filter (positive numbers only)\n");
                // Для демонстрации, предположим, что у нас есть функция фильтрации
                where(current_list, positiveFilterFunction);
                printf("Note: Filter function needs to be implemented separately.\n");
                break;
                
            case 7: // Применение map
                if (current_list == NULL) {
                    printf("Error: No list created.\n");
                    break;
                }
                // Предполагаем, что есть функция для отображения, например, удвоение
                printf("Applying map (doubling values)\n");
                // Для демонстрации, предположим, что у нас есть функция отображения
                map(current_list, doubleValueFunction);
                printf("Note: Map function needs to be implemented separately.\n");
                break;
                
            case 8: // Вывод всех элементов
                if (current_list == NULL) {
                    printf("Error: No list created.\n");
                    break;
                }
                printf("List elements: ");
                for (int i = 0; i < current_list->size; i++) {
                    double* element = (double*)get(current_list, i);
                    if (element != NULL) {
                        printf("%.2f ", *element);
                    }
                }
                printf("\n");
                break;
                
            case 9: // Удаление листа и всех его элементов
                clearList();
                printf("List and all elements deleted.\n");
                break;
                
            case 0: // Выход
                if (current_list != NULL) {
                    clearList();
                }
                printf("Exiting double UI...\n");
                return;
                
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }
}

// Консольный интерфейс для работы со строками
void stringUI() {

    int choice;
    char buffer[256];
    int index;
    
    while(1) {
        printf("\n=== String List Menu ===\n");
        printf("1. Create list\n");
        printf("2. Add element\n");
        printf("3. Get element\n");
        printf("4. Sort list\n");
        printf("5. Concatenate list with itself\n");
        printf("6. Apply where\n");
        printf("7. Apply map\n");
        printf("8. Print all elements\n");
        printf("9. Delete list and all elements\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        
        scanf("%d", &choice);
        getchar(); // Пропуск символа новой строки после числа
        
        switch(choice) {
            case 1: // Создание листа
                if (current_list != NULL) {
                    printf("Warning: Current list will be deleted. ");
                    clearList();
                }
                current_list = createEmptyList(getStringFieldInfo());
                printf("String list created successfully.\n");
                break;
                
            case 2: // Добавление элемента
                if (current_list == NULL) {
                    printf("Error: No list created. Create a list first.\n");
                    break;
                }
                printf("Enter string to add: ");
                fgets(buffer, sizeof(buffer), stdin);
                // Удаляем символ новой строки
                buffer[strcspn(buffer, "\n")] = 0;
                add(current_list, buffer);
                printf("Element added successfully.\n");
                break;
                
            case 3: // Получение элемента
                if (current_list == NULL) {
                    printf("Error: No list created.\n");
                    break;
                }
                printf("Enter index to get element: ");
                scanf("%d", &index);
                if (index >= 0 && index < current_list->size) {
                    char* element = (char*)get(current_list, index);
                    if (element != NULL) {
                        printf("Element at index %d: %s\n", index, element);
                    } else {
                        printf("Error getting element.\n");
                    }
                } else {
                    printf("Invalid index.\n");
                }
                break;
                
            case 4: // Сортировка листа
                if (current_list == NULL) {
                    printf("Error: No list created.\n");
                    break;
                }
                sort(current_list);
                printf("List sorted successfully.\n");
                break;
                
            case 5: // Конкатенация листа с самим собой
                if (current_list == NULL) {
                    printf("Error: No list created.\n");
                    break;
                }
                {
                    List* temp = concat(current_list, current_list);
                    if (temp != NULL) {
                        // Освобождаем старый список
                        for (int i = 0; i < current_list->size; i++) {
                            current_list->field_info->deallocate(current_list->data[i]);
                        }
                        free(current_list->data);
                        free(current_list);
                        
                        // Присваиваем новый конкатенированный список
                        current_list = temp;
                        printf("List concatenated with itself successfully.\n");
                    } else {
                        printf("Error concatenating lists.\n");
                    }
                }
                break;
                
            case 6: // Применение where
                if (current_list == NULL) {
                    printf("Error: No list created.\n");
                    break;
                }
                printf("Applying where filter (strings longer than 3 chars)\n");
                // Для демонстрации, предположим, что у нас есть функция фильтрации
                where(current_list, stringLengthFilterFunction);
                printf("Note: Filter function needs to be implemented separately.\n");
                break;
                
            case 7: // Применение map
                if (current_list == NULL) {
                    printf("Error: No list created.\n");
                    break;
                }
                printf("Applying map (converting to uppercase)\n");
                // Для демонстрации, предположим, что у нас есть функция отображения
                map(current_list, toUppercaseFunction);
                printf("Note: Map function needs to be implemented separately.\n");
                break;
                
            case 8: // Вывод всех элементов
                if (current_list == NULL) {
                    printf("Error: No list created.\n");
                    break;
                }
                printf("List elements: [");
                for (int i = 0; i < current_list->size; i++) {
                    char* element = (char*)get(current_list, i);
                    if (element != NULL) {
                        printf("\"%s\"", element);
                    }
                }
                printf("]\n");
                break;
                
            case 9: // Удаление листа и всех его элементов
                clearList();
                printf("List and all elements deleted.\n");
                break;
                
            case 0: // Выход
                printf("Exiting string UI...\n");
                return;
                
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }
}