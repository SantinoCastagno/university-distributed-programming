#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include "utils.h"

#define MAX_CONNECTIONS 5
#define MAX_LINE_LENGTH 100
#define SIZE_MESSAGE 1024
#define FILE_ENV_PATH "../env/env_server_horoscope.txt"

char *options[] = {"You will embark on an exciting adventure next year.",
                   "A new opportunity will present itself to you soon.",
                   "Your hard work will pay off in unexpected ways.",
                   "A significant change is on the horizon for you.",
                   "Someone from your past will re-enter your life.",
                   "You will make a valuable connection that will change your path.",
                   "Travel will be in your future, leading to new experiences.",
                   "Your creativity will flourish, leading to exciting projects.",
                   "An unexpected gift will bring joy to your life.",
                   "You will find inner peace through self-discovery."};
int numOptions;

// Function to select a random string from an array of strings
// TODO: resolve pointers problem
char *selectRandomString(char *strings[], int numStrings)
{
    // Generate a seed for the rand() function based on the current time
    srand(time(NULL));

    // Generate a random index between 0 and numStrings-1
    int randomIndex = rand() % numStrings;

    // Return the string corresponding to the random index
    return strings[randomIndex];
}

// function executed for each thread
void *connection_handler(void *socket_desc)
{
    int client_socket = *(int *)socket_desc, status, client_fd, valread;
    struct sockaddr_in serv_addr;
    char *buffer, buffer_client[SIZE_MESSAGE] = {0};

    // Extract message
    if (read(client_socket, buffer_client, SIZE_MESSAGE) == -1)
    {
        printf("ERROR\n");
        exit(EXIT_FAILURE);
    }

    printf("LOG: Handling request.\n");
    sleep(8);

    // Generate response, select a random string from the options
    buffer = selectRandomString(options, numOptions);

    // Send message to the client
    write(client_socket, buffer, strlen(buffer));
    printf("LOG: Response sent.\n");

    // close connection to the client
    close(client_socket);
    printf("LOG: Connection closed.\n");
    free(socket_desc);
    pthread_exit(NULL);
}

int main()
{
    int server_fd, client_socket, *new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    numOptions = sizeof(options) / sizeof(options[0]);

    if (set_env_vars(FILE_ENV_PATH))
    {
        printf("Error: enviroment vars dont setted correctly.\n");
        exit(EXIT_FAILURE);
    }

    // Socket creation
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(atoi(getenv("PORT")));
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
    printf("LOG: Listening in PORT %d.\n", atoi(getenv("PORT")));
    while (1)
    {
        // Accept the connection instance
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("Connection failed\n");
            exit(EXIT_FAILURE);
        }
        printf("LOG: Conneccion instance accepted.\n");
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
        printf("LOG: Handling connection.\n");
    }

    return 0;
}
