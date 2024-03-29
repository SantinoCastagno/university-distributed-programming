#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CONNECTIONS 10
#define PORT_HOR_SERVER 9090
#define PORT_WEA_SERVER 9091

// function executed for each thread
void *connection_handler(void *socket_desc)
{
    int client_socket = *(int *)socket_desc, status, client_hs_fd, client_we_fd, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};

    read(client_socket, buffer, 1024);
    printf("SC - Message received:%s\n", buffer);

    // Socket creation to the horoscope server
    if ((client_hs_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Socket creation error \n");
        exit(EXIT_FAILURE);
    }

    // Configure horoscope server params
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT_HOR_SERVER);
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("Invalid address/ Address not supported \n");
        exit(EXIT_FAILURE);
    }

    // Stablish connection to the horoscope server
    if ((status = connect(client_hs_fd, (struct sockaddr *)&serv_addr,
                          sizeof(serv_addr))) < 0)
    {
        printf("Connection Failed \n");
        exit(EXIT_FAILURE);
    }

    // Socket creation to the weather server
    if ((client_we_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Socket creation error \n");
        exit(EXIT_FAILURE);
    }

    // Configure weather server params
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT_HOR_SERVER);
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("Invalid address/ Address not supported \n");
        exit(EXIT_FAILURE);
    }

    // Stablish connection to the weather server
    if ((status = connect(client_we_fd, (struct sockaddr *)&serv_addr,
                          sizeof(serv_addr))) < 0)
    {
        printf("Connection Failed \n");
        exit(EXIT_FAILURE);
    }

    // TODO: EXTRACT PARAMS

    // TODO: CHECK PARAMS IN CACHE

    // TODO: SEND PARAMS TO THE SERVERS
    // Send message to the horoscope server
    send(client_hs_fd, hello, strlen(hello), 0);
    printf("CS: Hello message sent to the HS\n");

    // Send message to the weather server
    send(client_we_fd, hello, strlen(hello), 0);
    printf("CS: Hello message sent to the HS\n");

    // Read answer from the horoscope server
    valread = read(client_hs_fd, buffer, 1024 - 1);
    printf("CS: %s\n", buffer);

    // Read answer from the weather server
    valread = read(client_we_fd, buffer, 1024 - 1);
    printf("CS: %s\n", buffer);

    // TODO: merge the answer of the servers
    // Send message to the client
    write(client_socket, buffer, strlen(buffer));

    // close connection to the client
    close(client_socket);
    free(socket_desc);
    pthread_exit(NULL);
}

int main()
{
    int server_fd, client_socket, *new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Socket creation
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_fd, MAX_CONNECTIONS) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server found in PORT %d...\n", PORT);

    while (1)
    {
        // Accept the connection instance
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("Connection failed");
            exit(EXIT_FAILURE);
        }

        printf("Connection accepted\n");

        // Create a new socket for the new client
        new_socket = malloc(1);
        *new_socket = client_socket;

        // Create a new thread
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, connection_handler, (void *)new_socket) < 0)
        {
            perror("Thread creation failed");
            exit(EXIT_FAILURE);
        }

        printf("Handler assigned\n");
    }

    return 0;
}
