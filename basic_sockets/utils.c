#include <stdio.h>
#include <stdlib.h>

struct Date
{
    int year;
    int month;
    int day;
};

struct Date stringToDate(char *dateString)
{
    // Declaración de la estructura Date para almacenar la fecha desglosada
    struct Date clientDate;

    // Variables temporales para almacenar los componentes de la fecha
    int year, month, day;

    // Utiliza la función sscanf para desglosar el string de fecha
    if (sscanf(dateString, "%d-%d-%d", &day, &month, &year) != 3)
    {
        printf("Error: incorrect format of the date.\n");
        exit(EXIT_FAILURE);
    }

    if (year < 0 || year > 99 || day < 0 || day > 99 || month < 0 || month > 99)
    {
        printf("Error: some value of the date is incorrect.");
        exit(EXIT_FAILURE);
    }

    // Asigna los componentes desglosados a la estructura Date
    clientDate.year = year;
    clientDate.month = month;
    clientDate.day = day;

    return clientDate;
}
