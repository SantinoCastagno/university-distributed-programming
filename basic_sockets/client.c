#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

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

int main(int argc, char const *argv[])
{
    // Client variables
    int status, valread, client_fd;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    const char del[] = "-";
    char *token;

    // Verify params
    if (argc != 3)
    {
        printf("Cantidad de parámetros incorrecta\n");
        return 1;
    }

    printf("Cantidad de parametros correcta\n");
    printf("%d\n", argc);
    printf("%s\n", argv[1]);
    printf("%s\n", argv[2]);
    // TODO: fix this
    do {
        token = strtok(argv[2], del);
        printf("%s\n", token);
    } while (token != NULL);
    return 0; // FINISH

    // Socket creation
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    // Configure server params
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // Stablish connection to the server
    if ((status = connect(client_fd, (struct sockaddr *)&serv_addr,
                          sizeof(serv_addr))) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
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
