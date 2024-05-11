#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#include "utils.h"

#define SIZE_MESSAGE 1024
#define MAX_LINE_LENGTH 100

int set_env_vars()
{
    FILE *archivo;
    char linea[MAX_LINE_LENGTH];
    char *variable, *valor;

    // Abrir el archivo
    archivo = fopen("../env/env_client.txt", "r");
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

int main(int argc, char *argv[])
{
    // Client variables
    int status, valread, client_fd;
    struct sockaddr_in serv_addr;
    char buffer_send[SIZE_MESSAGE] = {0}, buffer_recv[SIZE_MESSAGE] = {0};
    char *token;
    struct Date paramDate;

    if (set_env_vars())
    {
        printf("Error: enviroment vars don't setted correctly.\n");
        exit(EXIT_FAILURE);
    }

    // Verify params
    if (argc != 3)
    {
        printf("Error: params incorrects\n");
        exit(EXIT_FAILURE);
    }

    // Check zodiac sign
    if (strcmp(argv[1], S1) != 0 && strcmp(argv[1], S2) != 0 && strcmp(argv[1], S3) != 0 && strcmp(argv[1], S4) != 0 && strcmp(argv[1], S5) != 0 && strcmp(argv[1], S6) != 0 && strcmp(argv[1], S7) != 0 && strcmp(argv[1], S8) != 0 && strcmp(argv[1], S9) != 0 && strcmp(argv[1], S10) != 0 && strcmp(argv[1], S11) != 0 && strcmp(argv[1], S12) != 0)
    {
        printf("Error: incorrect zodiac sign name.\n");
        exit(EXIT_FAILURE);
    }

    // Get and check date
    paramDate = stringToDate(argv[2]);

    // Concat both params to send one message
    sprintf(buffer_send, "%s %s", argv[1], argv[2]);

    // Socket creation
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Socket creation error \n");
        exit(EXIT_FAILURE);
    }

    // Configure server params
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(getenv("PORT")));
    if (inet_pton(AF_INET, getenv("IP"), &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        exit(EXIT_FAILURE);
    }

    // Stablish connection to the server
    if ((status = connect(client_fd, (struct sockaddr *)&serv_addr,
                          sizeof(serv_addr))) < 0)
    {
        printf("\nConnection Failed \n");
        exit(EXIT_FAILURE);
    }

    // Send message to the server
    send(client_fd, buffer_send, strlen(buffer_send), 0);

    // Read answer from the server
    valread = read(client_fd, buffer_recv, SIZE_MESSAGE - 1);
    printf("-------------------------- Predictions --------------------------\n%s-----------------------------------------------------------------\n", buffer_recv);

    // closing the connected socket
    close(client_fd);
    return 0;
}
