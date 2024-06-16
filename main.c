#include "phone_book.h"

#define OK 0
#define COMMAND_ERR 1
#define FILEPATH_ERR 2
#define ADD_EMPLOYEE_ERR 3
#define ANSWER_ERR 4
#define WRONG_ARGS_ERR 5

#define ANSWER_MAX 4

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Error. Incorrect number of args.");
        return WRONG_ARGS_ERR;
    }
    FILE *f;
    int command = -1;
    do
    {
        if (scanf("%d", &command) != 1 || command > 5 || command < 0)
        {
            printf("Error. Incorrect number of command.");
            return COMMAND_ERR;
        }

        if (command == 1)
        {
            f = fopen(PHONE_BOOK_FILEPATH, "r");
            if (f == NULL)
            {
                fclose(f);
                return FILEPATH_ERR;
            }

            print_data(f);
            fclose(f);
        }

        if (command == 2)
        {
            f = fopen(PHONE_BOOK_FILEPATH, "a");
            if (f == NULL)
            {
                fclose(f);
                return FILEPATH_ERR;
            }

            if (add_employee(f))
            {
                printf("Error. Incorrect data.");
                return ADD_EMPLOYEE_ERR;
            }
            fclose(f);
        }

        if (command == 3)
        {
            char substr[SURNAME_MAX];
            char answer[ANSWER_MAX];

            int c;
            while ((c = getchar())!= '\n' && c!= EOF);

            scan_str(substr, SURNAME_MAX, stdin);

            f = fopen(PHONE_BOOK_FILEPATH, "r");
            if (f == NULL)
            {
                fclose(f);
                return FILEPATH_ERR;
            }

            if (!fgets(answer, ANSWER_MAX, stdin))
                return ANSWER_ERR;

            if (strcmp(answer, "yes") == 0)
            {
                employee_t employees[EMPLOYEES_NUM_MAX];

                f = fopen(PHONE_BOOK_FILEPATH, "r+");
                if (f == NULL)
                {
                    fclose(f);
                    return FILEPATH_ERR;
                }
                size_t size = fill_arr_employees(f, employees);
                fclose(f);

                f = fopen(PHONE_BOOK_FILEPATH, "w");
                if (f == NULL)
                {
                    fclose(f);
                    return FILEPATH_ERR;
                }
                delete_by_substring(f, employees, size, substr);
                fclose(f);
            }
        }

        if (command == 4)
        {
            char substr[SURNAME_MAX];

            int c;
            while ((c = getchar())!= '\n' && c!= EOF);

            scan_str(substr, SURNAME_MAX, stdin);

            f = fopen(PHONE_BOOK_FILEPATH, "r+");

            find_by_substring(f, substr);

            fclose(f);
        }

        if (command == 5)
        {
            employee_t employees[EMPLOYEES_NUM_MAX];

            f = fopen(PHONE_BOOK_FILEPATH, "r");
            if (f == NULL)
            {
                fclose(f);
                return FILEPATH_ERR;
            }
            size_t size = fill_arr_employees(f, employees);
            fclose(f);

            sort(employees, size);

            f = fopen(PHONE_BOOK_FILEPATH, "w");
            if (f == NULL)
            {
                fclose(f);
                return FILEPATH_ERR;
            }

            fill_from_arr(f, employees, size);
            fclose(f);
        }

    }
    while (command != 0);
    return OK;
}

