#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100

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

int set_env_vars(char *file_path)
{
    FILE *archivo;
    char linea[MAX_LINE_LENGTH];
    char *variable, *valor;

    // Abrir el archivo
    archivo = fopen(file_path, "r");
    if (archivo == NULL)
    {
        perror("Error al abrir el archivo");
        return 1;
    }

    // Leer el archivo línea por línea
    while (fgets(linea, MAX_LINE_LENGTH, archivo) != NULL)
    {
        // Eliminar el salto de línea al final de la línea (si existe)
        linea[strcspn(linea, "\n")] = '\0';

        // Dividir la línea en variable y valor usando el signo '=' como delimitador
        variable = strtok(linea, "=");
        valor = strtok(NULL, "=");

        // Establecer la variable de entorno
        if (variable != NULL && valor != NULL)
        {
            if (setenv(variable, valor, 1) != 0)
            {
                perror("Error al establecer la variable de entorno");
                fclose(archivo);
                return 1;
            }
        }
    }
    // Cerrar el archivo
    fclose(archivo);
    return 0;
}