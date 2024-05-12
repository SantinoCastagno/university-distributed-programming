#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "utils.h"

#define SIZE_MESSAGE 1024
#define FILE_ENV_PATH "../env/env_client.txt"

int main(int argc, char *argv[])
{
    // Client variables
    int status, valread, client_fd;
    struct sockaddr_in serv_addr;
    char buffer_send[SIZE_MESSAGE] = {0}, buffer_recv[SIZE_MESSAGE] = {0};
    char *token;
    struct Date paramDate;

    if (set_env_vars(FILE_ENV_PATH))
    {
        printf("Error: enviroment vars don't setted correctly.\n");
        exit(EXIT_FAILURE);
    }

    printf("");

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
