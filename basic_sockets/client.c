#include <arpa/inet.h>
#include <sys/socket.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "utils.h"

#define PORT 8080
#define S1 "aries"
#define S2 "tauro"
#define S3 "geminis"
#define S4 "cancer"
#define S5 "leo"
#define S6 "virgo"
#define S7 "libra"
#define S8 "escorpio"
#define S9 "sagitario"
#define S10 "capricornio"
#define S11 "acuario"
#define S12 "piscis"

struct Date
{
    int year;
    int month;
    int day;
};

int main(int argc, char *argv[])
{
    // Client variables
    int status, valread, client_fd;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    char *token;
    struct Date paramDate;

    // Verify params
    if (argc != 3)
    {
        printf("Error: params incorrects\n");
        exit(EXIT_FAILURE);
    }

    // Get and check date
    paramDate = stringToDate(argv[2]);

    // Check zodiac sign
    if (strcmp(argv[1], S1) != 0 && strcmp(argv[1], S2) != 0 && strcmp(argv[1], S3) != 0 && strcmp(argv[1], S4) != 0 && strcmp(argv[1], S5) != 0 && strcmp(argv[1], S6) != 0 && strcmp(argv[1], S7) != 0 && strcmp(argv[1], S8) != 0 && strcmp(argv[1], S9) != 0 && strcmp(argv[1], S10) != 0 && strcmp(argv[1], S11) != 0 && strcmp(argv[1], S12) != 0)
    {
        printf("Error: incorrect zodiac sign name.\n");
        exit(EXIT_FAILURE);
    }

    // TODO: concat both params to send one message

    // Socket creation
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        exit(EXIT_FAILURE);
    }

    // Configure server params
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
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
    send(client_fd, hello, strlen(hello), 0);
    printf("Client: Hello message sent\n");

    // Read answer from the server
    valread = read(client_fd, buffer, 1024 - 1);
    printf("Client: %s\n", buffer);

    // closing the connected socket
    close(client_fd);
    return 0;
}
