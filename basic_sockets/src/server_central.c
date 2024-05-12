#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "utils.h"
#include "dictionary.h"

#define MAX_LINE_LENGTH 100
#define MAX_CONNECTIONS 10
#define SIZE_MESSAGE 1024
#define SIZE_DICTIONARY 100
#define MESSAGE_ERROR "ERROR: incorrect request format\n"
#define FILE_ENV_PATH "../env/env_server_central.txt"

sem_t dic_mutex;

typedef struct
{
    int *client_socket_ptr;
    Dictionary *dic;
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
    Dictionary *dic = (Dictionary *)thread_args->dic;
    struct Date message_date;

    // Extract message
    read(*client_socket, buffer_client, SIZE_MESSAGE);

    printf("LOG: Open connection.\n");
    sleep(1);

    // Check message receive
    if (sscanf(buffer_client, "%s %s", &sign, &date) != 2)
    {
        printf("Error: incorrect format of the message.\n");
        write(*client_socket, MESSAGE_ERROR, strlen(MESSAGE_ERROR));
        close(*client_socket);
        free(client_socket);
        pthread_exit(NULL);
    }

    // Check zodiac sign
    if (strcmp(sign, S1) != 0 && strcmp(sign, S2) != 0 && strcmp(sign, S3) != 0 && strcmp(sign, S4) != 0 && strcmp(sign, S5) != 0 && strcmp(sign, S6) != 0 && strcmp(sign, S7) != 0 && strcmp(sign, S8) != 0 && strcmp(sign, S9) != 0 && strcmp(sign, S10) != 0 && strcmp(sign, S11) != 0 && strcmp(sign, S12) != 0)
    {
        printf("Error: incorrect zodiac sign name.\n");
        write(*client_socket, MESSAGE_ERROR, strlen(MESSAGE_ERROR));
        close(*client_socket);
        free(client_socket);
        pthread_exit(NULL);
    }

    // Get and check date
    message_date = stringToDate(date);

    // TODO: CHECK PARAMS IN CACHE
    sem_wait(&dic_mutex);
    strcpy(value_sign, get(dic, sign));
    strcpy(value_date, get(dic, date));
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
        serv_addr.sin_port = htons(atoi(getenv("PORT_HOR_SERVER")));
        printf("%s",getenv("IP_HOR_SERVER"));
        if (inet_pton(AF_INET, getenv("IP_HOR_SERVER"), &serv_addr.sin_addr) <= 0)
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
        insert(dic, sign, buffer_horoscope);
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
        serv_addr.sin_port = htons(atoi(getenv("PORT_WEA_SERVER")));
        if (inet_pton(AF_INET, getenv("IP_WEA_SERVER"), &serv_addr.sin_addr) <= 0)
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
        printf("DEBUG: inserting weather in the cache.\t%lu\n", pthread_self());
        sleep(8);
        insert(dic, date, buffer_weather);
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
    Dictionary *dic = create_dictionary(SIZE_DICTIONARY);
    int *client_socket;
    sem_init(&dic_mutex, 0, 1);

    if (set_env_vars(FILE_ENV_PATH))
    {
        printf("ERROR: enviroment vars don't setted correctly.\n");
        exit(EXIT_FAILURE);
    }

    // Socket creation
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("ERROR: Socket creation failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(atoi(getenv("PORT")));
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

    printf("LOG: Listening in PORT %d.\n", atoi(getenv("PORT")));

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
        if (pthread_create(&thread_id, NULL, connection_handler, (void *)&args) < 0)
        {
            perror("Thread creation failed");
            exit(EXIT_FAILURE);
        }

        printf("LOG: Handler assigned\n");
    }
    free_dictionary(dic);

    return 0;
}
