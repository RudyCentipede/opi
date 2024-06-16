#include "phone_book.h"

/**
 * @brief Checks if a given string is a valid name.
 *
 * This function takes a string as input and checks if it is a valid name.
 * A valid name should only contain alphabetic characters and spaces.
 *
 * @param str The string to be checked.
 * @return true if the string is a valid name, false otherwise.
 */
bool is_name(char str[])
{
    bool res = true;
    for (size_t i = 0; str[i] != '\0'; i++)
        if (!isalpha(str[i]))
            res = false;
    return res;
}

/**
 * @brief Checks if a given string is a valid phone number.
 *
 * This function takes a string as input and checks if it represents a valid phone number.
 * Format: DDD-DDD, where D is a digit.
 *
 * @param str The string to be checked.
 * @return true if the string is a valid phone number, false otherwise.
 */
bool is_phone_number(char str[])
{
    bool res = true;
    if (strcmp(&str[3], "-") != 0)
        return res;
    for (size_t i = 0; str[i] != '\0'; i++)
        if (!isdigit(str[i]) || str[i] == '-')
            res = false;
    return res;
}

/**
 * @brief Checks if a given string represents a phone type.
 *
 * This function takes a string as input and checks if it represents a valid phone type.
 * A phone type is considered valid if it matches a predefined set of phone types: "ws", "wa", or "mb".
 * ws - work stationary, wa - work additional, mb - mobile.
 *
 * @param str The string to be checked.
 * @return true if the string represents a valid phone type, false otherwise.
 */
bool is_phone_type(char str[])
{
    return strcmp(str, "ws") == 0 || strcmp(str, "wa") == 0 || strcmp(str, "mb") == 0;
}


/**
 * @brief Scans a string from a file.
 *
 * This function reads a string from the file and stores it in the character array.
 *
 * @param str The character array to store the scanned string.
 * @param n The maximum number of characters to read, including the null terminator.
 * @param f The file pointer to read from.
 * @return 0 if the scan was successful, or a non-zero value if an error occurred.
 */
int scan_str(char str[], size_t n, FILE *f)
{
    if (n >= DEPARTMENT_MAX + 1)
        return 1;
    char buf[DEPARTMENT_MAX + 1];
    if (!fgets(buf, (int)(n + 1), f))
        return 1;

    size_t len = strlen(buf);
    if (buf[len - 1] == '\n')
    {
        len--;
        buf[len] = '\0';
    }

    if (len == 0 || len >= n)
        return 1;

    if ((n == SURNAME_MAX || n == NAME_MAX || n == PATRONYM_MAX) && !is_name(buf))
        return 1;
    if (n == PHONE_NUM_MAX && !is_phone_number(buf))
        return 1;
    if (n == PHONE_NUM_TYPE_MAX && !is_phone_type(buf))
        return 1;

    strncpy(str, buf, n);
    return 0;
}

/**
 * Scans an employee record from a file.
 *
 * This function reads data from the file and populates the given employee structure
 * with the scanned values.
 *
 * @param f The file pointer to read from.
 * @param employee A pointer to the employee structure to populate.
 * @return Returns 0 if the scan was successful, or a non-zero value if an error occurred.
 */
int scan_employee(FILE *f, employee_t *employee)
{
    if (scan_str(employee->surname, SURNAME_MAX, f))
        return 1;
    if (scan_str(employee->name, NAME_MAX, f))
        return 1;
    if (scan_str(employee->patronym, PATRONYM_MAX, f))
        return 1;
    if (scan_str(employee->department, DEPARTMENT_MAX, f))
        return 1;
    if (scan_str(employee->phone.number, PHONE_NUM_MAX, f))
        return 1;
    if (scan_str(employee->phone.type, PHONE_NUM_TYPE_MAX, f))
        return 1;

    return 0;
}

/**
 * @brief Prints the details of an employee to a file.
 *
 * This function prints the details of the given employee to the file.
 *
 * @param f The file pointer to which the employee details will be printed.
 * @param employee The employee whose details will be printed.
 */
void print_employee(FILE *f, employee_t employee)
{
    fprintf(f, "%s\n", employee.surname);
    fprintf(f, "%s\n", employee.name);
    fprintf(f, "%s\n", employee.patronym);
    fprintf(f, "%s\n", employee.department);
    fprintf(f, "%s\n", employee.phone.number);
    fprintf(f, "%s\n", employee.phone.type);

}

/**
 * @brief Prints the data from the given file.
 *
 * This function reads employees data from the file and prints it to the console.
 *
 * @param f The file pointer to read data from.
 */
void print_data(FILE *f)
{
    FILE *tmp = f;
    employee_t employee = { 0 };
    for (size_t i = 0; !scan_employee(tmp, &employee); i++)
    {
        print_employee(stdout, employee);
        tmp = f;
    }
}

/**
 * @brief Adds an employee to the phone book.
 *
 * This function takes a file pointer as a parameter and adds an employee to the phone book.
 *
 * @param f The file pointer to the phone book file.
 * @return 0 if the employee was added successfully, or a non-zero value if an error occurred.
 */
int add_employee(FILE *f)
{
    employee_t employee = { 0 };

    int c;
    while ((c = getchar())!= '\n' && c!= EOF);

    if (scan_employee(stdin, &employee) != 0)
        return 1;

    print_employee(f, employee);
    return 0;
}

/**
 * @brief Reads employee data from a file and fills an array of employee structures.
 *
 * This function reads employee data from the file and populates an array of employee structures.
 * The file should be opened in read mode before calling this function.
 *
 * @param f The file pointer to read employee data from.
 * @param employees The array of employee structures to fill with data.
 * @return The number of employees.
 */
size_t fill_arr_employees(FILE *f, employee_t employees[])
{
    size_t n = 0;
    employee_t employee = { 0 };
    for (size_t i = 0; !scan_employee(f, &employee); i++)
    {
        n++;
        memcpy(&employees[i], &employee, sizeof(employee_t));
    }
    return n;
}

/**
 * Deletes employees from the phone book whose names contain a given substring.
 *
 * @param f The file pointer to the phone book.
 * @param employees An array of employee_t structures representing the phone book.
 * @param n The number of employees in the phone book.
 * @param substr The substring to search for in employee names.
 */
void delete_by_substring(FILE *f, employee_t employees[], size_t n, char substr[])
{
    char cur_surname[SURNAME_MAX];
    size_t len_sb = strlen(substr);
    for (size_t i = 0; i < n; i++)
    {
        strcpy(cur_surname, employees[i].surname);
        cur_surname[len_sb] = '\0';
        if (strcmp(substr, cur_surname) != 0)
            print_employee(f, employees[i]);
    }
}

/**
 * @brief Finds a employees in a file.
 *
 *
 * This function searches for a given substring in a file and prints the details of employees
 * whose surnames start with the substring.
 *
 * @param f The file to search in.
 * @param substr The substring to search for.
 */
void find_by_substring(FILE *f, char substr[])
{
    char cur_surname[SURNAME_MAX];
    FILE *tmp = f;
    employee_t employee = { 0 };
    size_t len_sb = strlen(substr);
    for (size_t i = 0; !scan_employee(tmp, &employee); i++)
    {
        strcpy(cur_surname, employee.surname);
        cur_surname[len_sb] = '\0';
        if (strcmp(substr, cur_surname) == 0)
            print_employee(stdout, employee);
        tmp = f;
    }
    fclose(tmp);
}

/**
 * @brief Checks if one employee is greater than another.
 *
 * This function compares two employees and determines if emp1 is greater than emp2.
 * The comparison is based on the alphabetical order of the employee's surname, name, and patronym.
 *
 * @param emp1 The first employee to compare.
 * @param emp2 The second employee to compare.
 * @return true if emp1 is greater than emp2, false otherwise.
 */
bool is_greater(employee_t emp1, employee_t emp2)
{
    bool res = strcmp(emp1.department, emp2.department) > 0;
    res = res || (strcmp(emp1.department, emp2.department) == 0 && strcmp(emp1.surname, emp2.surname) > 0);
    if (!res)
        res = (strcmp(emp1.department, emp2.department) == 0 && strcmp(emp1.surname, emp2.surname) == 0 && strcmp(emp1.name, emp2.name) > 0);
    if (!res)
        res = (strcmp(emp1.department, emp2.department) == 0 && strcmp(emp1.surname, emp2.surname) == 0 && strcmp(emp1.name, emp2.name) == 0 && strcmp(emp1.patronym, emp2.patronym) > 0);

    return res;
}

/**
 * @brief Sorts an array of employee records.
 *
 * This function sorts the given array of employee records.
 * @param employees The array of employee records to be sorted.
 * @param n The number of elements in the array.
 */
void sort(employee_t employees[], size_t n)
{
    employee_t temp;
    int end = 0;
    for (size_t i = 0; i < n - 1 && !end; i++)
    {
        end = 1;
        for (size_t j = 0; j < n - 1 - i; j++)
            if (is_greater(employees[j], employees[j + 1]))
            {
                end = 0;
                memcpy(&temp, &employees[j], sizeof(employee_t));
                memcpy(&employees[j], &employees[j + 1], sizeof(employee_t));
                memcpy(&employees[j + 1], &temp, sizeof(employee_t));
            }
    }
}


/**
 * @brief Writes employee data from an array to a file.
 *
 * This function takes an array of employee data and writes it to the file.
 *
 * @param f The file pointer to write the data to.
 * @param employees The array of employee data.
 * @param n The number of employees in the array.
 */
void fill_from_arr(FILE *f, employee_t employees[], size_t n)
{
    for (size_t i = 0; i < n; i++)
        print_employee(f, employees[i]);
}
