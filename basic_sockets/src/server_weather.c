#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_CONNECTIONS 5
#define MAX_LINE_LENGTH 100
#define SIZE_MESSAGE 1024

char *options[] = {"Sunny with clear skies and warm temperatures.",
                   "Partly cloudy with a chance of scattered showers in the afternoon.",
                   "Overcast with intermittent rain throughout the day.",
                   "Mostly sunny with a light breeze from the northwest.",
                   "Thunderstorms likely in the evening, with heavy rainfall expected.",
                   "Foggy conditions in the morning, gradually clearing up by midday.",
                   "A mix of sun and clouds, with mild temperatures and no precipitation.",
                   "Windy conditions expected, especially in coastal areas.",
                   "Freezing rain and icy roads possible, use caution when driving.",
                   "Clear skies overnight with temperatures dropping below freezing."};
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

int set_env_vars()
{
    FILE *archivo;
    char linea[MAX_LINE_LENGTH];
    char *variable, *valor;

    // Abrir el archivo
    archivo = fopen("../env/env_server_weather.txt", "r");
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

    if (set_env_vars())
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
