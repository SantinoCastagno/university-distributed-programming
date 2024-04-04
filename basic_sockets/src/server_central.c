#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <semaphore.h>

#include "dictionary.h"

#define PORT 8080
#define MAX_CONNECTIONS 10
#define PORT_HOR_SERVER 9090
#define PORT_WEA_SERVER 9091

#define SIZE_MESSAGE 1024
#define SIZE_DICTIONARY 100

sem_t dic_mutex;

typedef struct
{
    int *client_socket_ptr;
    Diccionario *dic;
} ThreadArgs;

// function executed for each thread
void *connection_handler(void *args)
{
    int status, client_hs_fd, client_we_fd, valread;
    struct sockaddr_in serv_addr;
    char buffer_weather[SIZE_MESSAGE] = "request", buffer_horoscope[SIZE_MESSAGE] = "request", buffer_client[SIZE_MESSAGE] = {0}, sign[32], date[12], value_sign[64], value_date[64];

    // args init
    ThreadArgs *thread_args = (ThreadArgs *)args;
    int *client_socket = (int *)thread_args->client_socket_ptr;
    Diccionario *dic = (Diccionario *)thread_args->dic;

    // Extract message
    read(*client_socket, buffer_client, SIZE_MESSAGE);

    printf("LOG: Open connection.\n");
    sleep(2);

    // Check message receive
    if (sscanf(buffer_client, "%s %s", &sign, &date) != 2)
    {
        printf("Error: incorrect format of the date.\n");
        exit(EXIT_FAILURE);
    }

    // TODO: CHECK PARAMS IN CACHE
    sem_wait(&dic_mutex);
    strcpy(value_sign, obtener(dic, sign));
    strcpy(value_date, obtener(dic, date));
    sem_post(&dic_mutex);

    if (strcmp(value_sign, "") == 0)
    {
        // Socket creation to the horoscope server
        if ((client_hs_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            perror("Socket creation error\n");
            exit(EXIT_FAILURE);
        }

        // Configure horoscope server params
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(PORT_HOR_SERVER);
        if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
        {
            perror("Invalid address/ Address not supported\n");
            exit(EXIT_FAILURE);
        }

        // Stablish connection to horoscope server
        if ((status = connect(client_hs_fd, (struct sockaddr *)&serv_addr,
                              sizeof(serv_addr))) < 0)
        {
            perror("Connection Failed\n");
            exit(EXIT_FAILURE);
        }

        // Send request to horoscope server
        send(client_hs_fd, buffer_horoscope, strlen(buffer_horoscope), 0);

        // Read answer from horoscope server
        valread = read(client_hs_fd, buffer_horoscope, SIZE_MESSAGE - 1);
        sem_wait(&dic_mutex);
        insertar(dic, sign, buffer_horoscope);
        sem_post(&dic_mutex);
    }
    else
    {
        strcpy(buffer_horoscope, value_sign);
    }

    if (strcmp(value_date, "") == 0)
    {
        // Socket creation to weather server
        if ((client_we_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            perror("Socket creation error\n");
            exit(EXIT_FAILURE);
        }

        // Configure weather server params
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(PORT_WEA_SERVER);
        if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
        {
            perror("Invalid address/ Address not supported\n");
            exit(EXIT_FAILURE);
        }

        // Stablish connection to weather server
        if ((status = connect(client_we_fd, (struct sockaddr *)&serv_addr,
                              sizeof(serv_addr))) < 0)
        {
            perror("ERROR: Connection failed");
            exit(EXIT_FAILURE);
        }

        // Send request to weather server
        send(client_we_fd, buffer_weather, strlen(buffer_weather), 0);

        // Read answer from eather server
        valread = read(client_we_fd, buffer_weather, SIZE_MESSAGE - 1);
        sem_wait(&dic_mutex);
        printf("LOG: Insertando clima en el diccionario.\t%lu\n", pthread_self());
        sleep(20);
        insertar(dic, date, buffer_weather);
        sem_post(&dic_mutex);
    }
    else
    {
        strcpy(buffer_weather, value_date);
    }

    // Merge the response from the servers
    sprintf(buffer_client, "%s\n%s\n", buffer_horoscope, buffer_weather);

    // Send response to the client
    write(*client_socket, buffer_client, strlen(buffer_client));
    printf("LOG: Response sent\n", dic);
    close(*client_socket);
    free(client_socket);
    pthread_exit(NULL);
}

int main()
{
    int server_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    Diccionario *dic = crear_diccionario(SIZE_DICTIONARY);
    int *client_socket;
    sem_init(&dic_mutex, 0, 1);

    // Socket creation
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("ERROR: Socket creation failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("ERROR: Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_fd, MAX_CONNECTIONS) < 0)
    {
        perror("ERROR: Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("LOG: Listening in PORT %d.\n", PORT);

    while (1)
    {
        // Accept the connection instance
        client_socket = malloc(4);
        if ((*client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("Connection failed");
            exit(EXIT_FAILURE);
        }

        // Create thread id and setting params
        pthread_t thread_id;
        ThreadArgs args = {client_socket, dic};
        printf("DEBUG: client_socket_ptr: \t%x \t%d\n", client_socket, *client_socket);
        if (pthread_create(&thread_id, NULL, connection_handler, (void *)&args) < 0)
        {
            perror("Thread creation failed");
            exit(EXIT_FAILURE);
        }

        printf("LOG: Handler assigned\n");
    }
    liberar_diccionario(dic);

    return 0;
}
