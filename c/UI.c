#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#include "../headers/FieldInfo.h"
#include "../headers/DynamicArray.h"
#include "../headers/UsersStruct.h"
#include "../headers/UI.h"

static DynamicArray *current_dynamic_array = NULL;

extern const FieldInfo *get_double_field_info(void);
extern const FieldInfo *get_string_field_info(void);
extern const FieldInfo *get_point_field_info(void);

static void clear_dynamic_array(void) {
    if (current_dynamic_array != NULL) {
        if (!destroy_dynamic_array(current_dynamic_array)) {
            perror("Warning: Failed to destroy dynamic array");
        }
        current_dynamic_array = NULL;
    }
}

static int read_int(const char *prompt) {
    int value = 0;
    int result;

    while (1) {
        printf("%s", prompt);
        result = scanf("%d", &value);
        if (result == 1) {
            while (getchar() != '\n') {}
            return value;
        }
        printf("Invalid input. Please enter an integer.\n\n");
        while (getchar() != '\n');
    }
}

static double read_double(const char *prompt) {
    double value = 0.0;
    int result;

    while (1) {
        printf("%s", prompt);
        result = scanf("%lf", &value);
        if (result == 1) {
            while (getchar() != '\n') {}
            return value;
        }
        printf("Invalid input. Please enter a real number.\n\n");
        while (getchar() != '\n');
    }
}

static bool positive_filter_function(const void *element) {
    double value = *(double *)element;
    return value > 0;
}

static void *double_value_function(void *element) {
    double *value = (double *)element;
    double *new_value = (double *)malloc(sizeof(double));
    if (new_value == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    *new_value = *value * 2;
    return new_value;
}

static bool string_length_filter_function(const void *element) {
    char *str = (char *)element;
    return str != NULL && strlen(str) > 3;
}

static void *to_uppercase_function(void *element) {
    char *original_str = (char *)element;
    if (original_str == NULL) {
        errno = EINVAL;
        return NULL;
    }

    size_t len = strlen(original_str);
    char *upper_str = malloc((len + 1) * sizeof(char));
    if (upper_str == NULL) {
        errno = ENOMEM;
        return NULL;
    }

    for (size_t i = 0; i <= len; i++) {
        if (original_str[i] >= 'a' && original_str[i] <= 'z') {
            upper_str[i] = original_str[i] - 'a' + 'A';
        } else {
            upper_str[i] = original_str[i];
        }
    }
    upper_str[len] = '\0';
    return upper_str;
}

static bool positive_point_filter_function(const void *element) {
    Point *p = (Point *)element;
    return p != NULL && p->x > 0 && p->y > 0 && p->z > 0;
}

static void *increment_point_function(void *element) {
    Point *point = (Point *)element;
    Point *new_point = (Point *)malloc(sizeof(Point));
    if (new_point == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    new_point->x = point->x + 1;
    new_point->y = point->y + 1;
    new_point->z = point->z + 1;
    return new_point;
}

static void print_double_array(void) {
    if (current_dynamic_array == NULL) {
        printf("Error: No dynamic array created.\n");
        return;
    }

    printf("Dynamic array elements: ");
    for (size_t i = 0; i < current_dynamic_array->size; i++) {
        double *element = (double *)get(current_dynamic_array, i);
        if (element != NULL) {
            printf("%.2f ", *element);
        }
    }
    printf("\n");
}

static void print_point_array(void) {
    if (current_dynamic_array == NULL) {
        printf("Error: No dynamic array created.\n");
        return;
    }

    printf("Dynamic array elements: ");
    for (size_t i = 0; i < current_dynamic_array->size; i++) {
        Point *element = (Point *)get(current_dynamic_array, i);
        if (element != NULL) {
            printf("(%.2f, %.2f, %.2f) ", element->x, element->y, element->z);
        }
    }
    printf("\n");
}

static void print_string_array(void) {
    if (current_dynamic_array == NULL) {
        printf("Error: No dynamic array created.\n");
        return;
    }

    printf("Dynamic array elements: [");
    for (size_t i = 0; i < current_dynamic_array->size; i++) {
        char *element = (char *)get(current_dynamic_array, i);
        if (element != NULL) {
            printf("\"%s\"", element);
            if (i < current_dynamic_array->size - 1) {
                printf(", ");
            }
        }
    }
    printf("]\n");
}

void double_ui(void) {
    int choice = 0;
    double value;
    int index;

    while (1) {
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

        choice = read_int("Enter your choice: ");

        switch (choice) {
            case 1:
                if (current_dynamic_array != NULL) {
                    printf("Warning: Current dynamicArray will be deleted. ");
                    clear_dynamic_array();
                }
                current_dynamic_array = create_empty_dynamic_array(get_double_field_info());
                if (current_dynamic_array == NULL) {
                    perror("Error: Failed to create dynamicArray");
                } else {
                    printf("Double dynamicArray created successfully.\n");
                }
                break;

            case 2:
                if (current_dynamic_array == NULL) {
                    printf("Error: No dynamicArray created. Create a dynamicArray first.\n");
                    break;
                }
                value = read_double("Enter double value to add: ");
                if (add(current_dynamic_array, &value)) {

                    printf("Element added successfully.\n");
                } else {
                    perror("Error: Failed to add element");
                }
                break;

            case 3:
                if (current_dynamic_array == NULL) {
                    printf("Error: No dynamicArray created.\n");
                    break;
                }
                index = read_int("Enter index to get element: ");
                if (index >= 0 && index < current_dynamic_array->size) {
                    double *element = (double *)get(current_dynamic_array, index);
                    if (element != NULL) {
                        printf("Element at index %d: %f\n", index, *element);
                    } else {
                        perror("Error getting element");
                    }
                } else {
                    printf("Invalid index. Please enter a number between 0 and %zu.\n",
                           current_dynamic_array->size - 1);
                }
                break;

            case 4:
                if (current_dynamic_array == NULL) {
                    printf("Error: No dynamicArray created.\n");
                    break;
                }
                if (sort(current_dynamic_array)) {
                    printf("DynamicArray sorted successfully.\n");
                } else {
                    perror("Error: Failed to sort dynamicArray");
                }
                break;

            case 5:
                if (current_dynamic_array == NULL) {
                    printf("Error: No dynamicArray created.\n");
                    break;
                }
                {
                    DynamicArray *temp = create_empty_dynamic_array(current_dynamic_array->field_info);
                    if (concat(current_dynamic_array, current_dynamic_array, temp)) {
                        if (!destroy_dynamic_array(current_dynamic_array)) {
                            perror("Warning: Failed to destroy old array");
                        }
                        current_dynamic_array = temp;
                        printf("DynamicArray concatenated with itself successfully.\n");
                    } else {
                        perror("Error concatenating dynamicArrays");
                    }
                }
                break;

            case 6:
                if (current_dynamic_array == NULL) {
                    printf("Error: No dynamicArray created.\n");
                    break;
                }
                printf("Applying where filter (positive numbers only)\n");
                {
                    DynamicArray *filtered = create_empty_dynamic_array(current_dynamic_array->field_info);
                    if (where(current_dynamic_array, positive_filter_function, filtered)) {
                        if (!destroy_dynamic_array(current_dynamic_array)) {
                            perror("Warning: Failed to destroy old array");
                        }
                        current_dynamic_array = filtered;
                        printf("Where filter applied successfully.\n");
                    } else {
                        perror("Error applying where filter");
                    }
                }
                break;

            case 7:
                if (current_dynamic_array == NULL) {
                    printf("Error: No dynamicArray created.\n");
                    break;
                }
                printf("Applying map (doubling values)\n");
                {
                    DynamicArray *mapped = create_empty_dynamic_array(current_dynamic_array->field_info);
                    if (map(current_dynamic_array, double_value_function, mapped)) {
                        if (!destroy_dynamic_array(current_dynamic_array)) {
                            perror("Warning: Failed to destroy old array");
                        }
                        current_dynamic_array = mapped;
                        printf("Map applied successfully.\n");
                    } else {
                        perror("Error applying map");
                    }
                }
                break;

            case 8:
                print_double_array();
                break;

            case 9:
                clear_dynamic_array();
                printf("DynamicArray and all elements deleted.\n");
                break;

            case 0:
                clear_dynamic_array();
                printf("Exiting double UI...\n");
                return;

            default:
                printf("Invalid choice. Please enter a number from the menu options.\n");
                break;
        }
    }
}

void point_ui(void) {
    int choice = 0;
    Point value;
    int index;

    while (1) {
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

        choice = read_int("Enter your choice: ");

        switch (choice) {
            case 1:
                if (current_dynamic_array != NULL) {
                    printf("Warning: Current dynamicArray will be deleted. ");
                    clear_dynamic_array();
                }
                current_dynamic_array = create_empty_dynamic_array(get_point_field_info());
                if (current_dynamic_array == NULL) {
                    perror("Error: Failed to create dynamicArray");
                } else {
                    printf("Point dynamicArray created successfully.\n");
                }
                break;

            case 2:
                if (current_dynamic_array == NULL) {
                    printf("Error: No dynamicArray created. Create a dynamicArray first.\n");
                    break;
                }
                value.x = read_double("Enter x coordinate: ");
                value.y = read_double("Enter y coordinate: ");
                value.z = read_double("Enter z coordinate: ");
                if (!add(current_dynamic_array, &value)) {
                    perror("Error: Failed to add element");
                } else {
                    printf("Element added successfully.\n");
                }
                break;

            case 3:
                if (current_dynamic_array == NULL) {
                    printf("Error: No dynamicArray created.\n");
                    break;
                }
                index = read_int("Enter index to get element: ");
                if (index >= 0 && index < (int)current_dynamic_array->size) {
                    Point *element = (Point *)get(current_dynamic_array, index);
                    if (element != NULL) {
                        printf("Element at index %d: (%.2f, %.2f, %.2f)\n",
                               index, element->x, element->y, element->z);
                    } else {
                        perror("Error getting element");
                    }
                } else {
                    printf("Invalid index. Please enter a number between 0 and %zu.\n",
                           current_dynamic_array->size - 1);
                }
                break;

            case 4:
                if (current_dynamic_array == NULL) {
                    printf("Error: No dynamicArray created.\n");
                    break;
                }
                if (!sort(current_dynamic_array)) {
                    perror("Error: Failed to sort dynamicArray");
                } else {
                    printf("DynamicArray sorted successfully.\n");
                }
                break;

            case 5:
                if (current_dynamic_array == NULL) {
                    printf("Error: No dynamicArray created.\n");
                    break;
                }
                {
                    DynamicArray *temp = create_empty_dynamic_array(current_dynamic_array->field_info);
                    if (concat(current_dynamic_array, current_dynamic_array, temp)) {
                        if (!destroy_dynamic_array(current_dynamic_array)) {
                            perror("Warning: Failed to destroy old array");
                        }
                        current_dynamic_array = temp;
                        printf("DynamicArray concatenated with itself successfully.\n");
                    } else {
                        perror("Error concatenating dynamicArrays");
                    }
                }
                break;

            case 6:
                if (current_dynamic_array == NULL) {
                    printf("Error: No dynamicArray created.\n");
                    break;
                }
                printf("Applying where filter (points with positive coordinates only)\n");
                {
                    DynamicArray *filtered = create_empty_dynamic_array(current_dynamic_array->field_info);
                    if (where(current_dynamic_array, positive_point_filter_function, filtered)) {
                        if (!destroy_dynamic_array(current_dynamic_array)) {
                            perror("Warning: Failed to destroy old array");
                        }
                        current_dynamic_array = filtered;
                        printf("Where filter applied successfully.\n");
                    } else {
                        perror("Error applying where filter");
                    }
                }
                break;

            case 7:
                if (current_dynamic_array == NULL) {
                    printf("Error: No dynamicArray created.\n");
                    break;
                }
                printf("Applying map (incrementing coordinates by 1)\n");
                {
                    DynamicArray *mapped = create_empty_dynamic_array(current_dynamic_array->field_info);
                    if (map(current_dynamic_array, increment_point_function, mapped)) {
                        if (!destroy_dynamic_array(current_dynamic_array)) {
                            perror("Warning: Failed to destroy old array");
                        }
                        current_dynamic_array = mapped;
                        printf("Map applied successfully.\n");
                    } else {
                        perror("Error applying map");
                    }
                }
                break;

            case 8:
                print_point_array();
                break;

            case 9:
                clear_dynamic_array();
                printf("DynamicArray and all elements deleted.\n");
                break;

            case 0:
                clear_dynamic_array();
                printf("Exiting point UI...\n");
                return;

            default:
                printf("Invalid choice. Please enter a number from the menu options.\n");
                break;
        }
    }
}

void string_ui(void) {
    int choice = 0;
    char buffer[256];
    int index;

    while (1) {
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

        choice = read_int("Enter your choice: ");

        switch (choice) {
            case 1:
                if (current_dynamic_array != NULL) {
                    printf("Warning: Current dynamicArray will be deleted. ");
                    clear_dynamic_array();
                }
                current_dynamic_array = create_empty_dynamic_array(get_string_field_info());
                if (current_dynamic_array == NULL) {
                    perror("Error: Failed to create dynamicArray");
                } else {
                    printf("String dynamicArray created successfully.\n");
                }
                break;

            case 2:
                if (current_dynamic_array == NULL) {
                    printf("Error: No dynamicArray created. Create a dynamicArray first.\n");
                    break;
                }
                printf("Enter string to add: ");
                if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                    perror("Error reading input");
                    break;
                }
                buffer[strcspn(buffer, "\n")] = 0;
                if (!add(current_dynamic_array, buffer)) {
                    perror("Error: Failed to add element");
                } else {
                    printf("Element added successfully.\n");
                }
                break;

            case 3:
                if (current_dynamic_array == NULL) {
                    printf("Error: No dynamicArray created.\n");
                    break;
                }
                index = read_int("Enter index to get element: ");
                if (index >= 0 && index < (int)current_dynamic_array->size) {
                    char *element = (char *)get(current_dynamic_array, index);
                    if (element != NULL) {
                        printf("Element at index %d: %s\n", index, element);
                    } else {
                        perror("Error getting element");
                    }
                } else {
                    printf("Invalid index. Please enter a number between 0 and %zu.\n",
                           current_dynamic_array->size - 1);
                }
                break;

            case 4:
                if (current_dynamic_array == NULL) {
                    printf("Error: No dynamicArray created.\n");
                    break;
                }
                if (!sort(current_dynamic_array)) {
                    perror("Error: Failed to sort dynamicArray");
                } else {
                    printf("DynamicArray sorted successfully.\n");
                }
                break;

            case 5:
                if (current_dynamic_array == NULL) {
                    printf("Error: No dynamicArray created.\n");
                    break;
                }
                {
                    DynamicArray *temp = create_empty_dynamic_array(current_dynamic_array->field_info);
                    if (concat(current_dynamic_array, current_dynamic_array, temp)) {
                        if (!destroy_dynamic_array(current_dynamic_array)) {
                            perror("Warning: Failed to destroy old array");
                        }
                        current_dynamic_array = temp;
                        printf("DynamicArray concatenated with itself successfully.\n");
                    } else {
                        perror("Error concatenating dynamicArrays");
                    }
                }
                break;

            case 6:
                if (current_dynamic_array == NULL) {
                    printf("Error: No dynamicArray created.\n");
                    break;
                }
                printf("Applying where filter (strings longer than 3 chars)\n");
                {
                    DynamicArray *filtered = create_empty_dynamic_array(current_dynamic_array->field_info);
                    if (where(current_dynamic_array, string_length_filter_function, filtered)) {
                        if (!destroy_dynamic_array(current_dynamic_array)) {
                            perror("Warning: Failed to destroy old array");
                        }
                        current_dynamic_array = filtered;
                        printf("Where filter applied successfully.\n");
                    } else {
                        perror("Error applying where filter");
                    }
                }
                break;

            case 7:
                if (current_dynamic_array == NULL) {
                    printf("Error: No dynamicArray created.\n");
                    break;
                }
                printf("Applying map (converting to uppercase)\n");
                {
                    DynamicArray *mapped = create_empty_dynamic_array(current_dynamic_array->field_info);
                    if (map(current_dynamic_array, to_uppercase_function, mapped)) {
                        if (!destroy_dynamic_array(current_dynamic_array)) {
                            perror("Warning: Failed to destroy old array");
                        }
                        current_dynamic_array = mapped;
                        printf("Map applied successfully.\n");
                    } else {
                        perror("Error applying map");
                    }
                }
                break;

            case 8:
                print_string_array();
                break;

            case 9:
                clear_dynamic_array();
                printf("DynamicArray and all elements deleted.\n");
                break;

            case 0:
                clear_dynamic_array();
                printf("Exiting string UI...\n");
                return;

            default:
                printf("Invalid choice. Please enter a number from the menu options.\n");
                break;
        }
    }
}