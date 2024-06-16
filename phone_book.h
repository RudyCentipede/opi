#ifndef PHONE_BOOK_H__
#define PHONE_BOOK_H__

#define SURNAME_MAX 26
#define NAME_MAX 11
#define PATRONYM_MAX 31
#define DEPARTMENT_MAX 41
#define PHONE_NUM_MAX 8
#define PHONE_NUM_TYPE_MAX 3
#define EMPLOYEES_NUM_MAX 100

#define PHONE_BOOK_FILEPATH argv[1]

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

/**
 * @struct phone_t
 * @brief Represents a phone number in a phone book.
 *
 * This struct contains the phone number and its type.
 */
typedef struct
{
    char number[PHONE_NUM_MAX];
    char type[PHONE_NUM_TYPE_MAX];
} phone_t;

/**
 * @struct employee_t
 * @brief Represents an employee in a phone book.
 *
 * This struct stores information about an employee, including their surname, name, patronym,
 * department, and phone number.
 */
typedef struct
{
    char surname[SURNAME_MAX];
    char name[NAME_MAX];
    char patronym[PATRONYM_MAX];
    char department[DEPARTMENT_MAX];
    phone_t phone;
} employee_t;

bool is_name(char str[]);
bool is_phone_number(char str[]);
bool is_phone_type(char str[]);
size_t fill_arr_employees(FILE *f, employee_t employees[]);
int scan_str(char str[], size_t n, FILE *f);
int scan_employee(FILE *f, employee_t *employee);
int add_employee(FILE *f);
void print_data(FILE *f);
void print_employee(FILE *f, employee_t employee);
void delete_by_substring(FILE *f, employee_t employees[], size_t n, char substr[]);
void find_by_substring(FILE *f, char substr[]);
void sort(employee_t employees[], size_t n);
void fill_from_arr(FILE *f, employee_t employees[], size_t n);

#endif // PHONE_BOOK_H__
