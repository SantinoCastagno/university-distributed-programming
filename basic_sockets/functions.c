#include <stdio.h>
#include <stdlib.h>

struct Date {
    int year;
    int month;
    int day;
};

struct Date stringToDate(char* dateString) {

    // Declaración de la estructura Date para almacenar la fecha desglosada
    struct Date clientDate;

    // Variables temporales para almacenar los componentes de la fecha
    int year, month, day;

    // Utiliza la función sscanf para desglosar el string de fecha
    if (sscanf(dateString, "%d-%d-%d", &day, &month, &year) != 3) {
        printf("Error al ingresar la fecha.\n");
        exit(EXIT_FAILURE);
    }

    // Asigna los componentes desglosados a la estructura Date
    clientDate.year = year;
    clientDate.month = month;
    clientDate.day = day;


    return clientDate;
}
