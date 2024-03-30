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

typedef struct
{
    int *client_socket_ptr;
    int *server_hor_ptr;
    int *server_wea_ptr;
} ThreadArgs;

// function executed for each thread
void *connection_handler(void *args)
{
    ThreadArgs *thread_args = (ThreadArgs *)args;
    int client_socket = *(int *)thread_args->client_socket_ptr;
    int server_hor_socket = *(int *)thread_args->server_hor_ptr;
    int server_wea_socket = *(int *)thread_args->server_wea_ptr;
    int status, client_hs_fd, client_we_fd, valread;
    struct sockaddr_in serv_addr;
    char buffer_weather[1024] = {0}, buffer_horoscope[1024], buffer_client[1024] = {0}, sign[32], date[12];

    // Extract message
    read(client_socket, buffer_client, 1024);

    // Check message receive
    if (sscanf(buffer_client, "%s %s", &sign, &date) != 2)
    {
        printf("Error: incorrect format of the date.\n");
        exit(EXIT_FAILURE);
    }

    // TODO: CHECK PARAMS IN CACHE

    // Send message to the other servers
    send(client_hs_fd, buffer_horoscope, strlen(buffer_horoscope), 0);
    send(client_we_fd, buffer_weather, strlen(buffer_weather), 0);

    // Read answer from the horoscope server
    valread = read(client_hs_fd, buffer_horoscope, 1024 - 1);
    printf("CS: %s\n", buffer_horoscope);

    // Read answer from the weather server
    valread = read(client_we_fd, buffer_weather, 1024 - 1);
    printf("CS: %s\n", buffer_weather);

    // TODO: merge the answer of the servers
    sprintf(buffer_client, "%s\n%s\n", buffer_horoscope, buffer_weather);

    // Send message to the client
    write(client_socket, buffer_client, strlen(buffer_client));

    // close connection to the client
    close(client_socket);
    free(thread_args->client_socket_ptr);
    pthread_exit(NULL);
}

int main()
{
    int listen_socket, *connection_socket_ptr, *server_hor_ptr, *server_wea_ptr;
    struct sockaddr_in address, serv_addr;
    int addrlen = sizeof(address), status;

    server_hor_ptr = malloc(1);
    server_wea_ptr = malloc(1);

    // Socket creation to the horoscope server
    if ((*server_hor_ptr = socket(AF_INET, SOCK_STREAM, 0)) < 0)
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
    if ((status = connect(*server_hor_ptr, (struct sockaddr *)&serv_addr,
                          sizeof(serv_addr))) < 0)
    {
        printf("Connection Failed \n");
        exit(EXIT_FAILURE);
    }

    // Socket creation to the weather server
    if ((*server_wea_ptr = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Socket creation error \n");
        exit(EXIT_FAILURE);
    }

    // Configure weather server params
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT_WEA_SERVER);
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("Invalid address/ Address not supported \n");
        exit(EXIT_FAILURE);
    }

    // Stablish connection to the weather server
    if ((status = connect(*server_wea_ptr, (struct sockaddr *)&serv_addr,
                          sizeof(serv_addr))) < 0)
    {
        printf("Connection Failed \n");
        exit(EXIT_FAILURE);
    }

    // Listening socket creation
    if ((listen_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(listen_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(listen_socket, MAX_CONNECTIONS) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server found in PORT %d...\n", PORT);

    while (1)
    {
        // Create a new socket for the new client
        connection_socket_ptr = malloc(1);
        // Accept the connection instance
        if ((*connection_socket_ptr = accept(listen_socket, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("Connection failed");
            exit(EXIT_FAILURE);
        }
        printf("Connection accepted\n");

        // Create a new thread
        pthread_t thread_id;
        ThreadArgs args = {connection_socket_ptr, server_hor_ptr, server_wea_ptr};
        if (pthread_create(&thread_id, NULL, connection_handler,(void *)&args) < 0)
        {
            perror("Thread creation failed");
            exit(EXIT_FAILURE);
        }

        printf("Handler assigned\n");
    }
    free(server_hor_ptr);
    free(server_wea_ptr);
    return 0;
}
