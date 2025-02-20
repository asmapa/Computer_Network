#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 20
#define BUFFER_SIZE 500

pthread_mutex_t mutex;
int clients[MAX_CLIENTS];
int n = 0;

void remove_client(int sock)
{
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < n; i++)
    {
        if (clients[i] == sock)
        {
            // Shift remaining clients down
            for (int j = i; j < n - 1; j++)
            {
                clients[j] = clients[j + 1];
            }
            n--;
            break;
        }
    }
    pthread_mutex_unlock(&mutex);
}

void send_to_all(char *msg, int curr_sock)
{
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < n; i++)
    {
        if (clients[i] != curr_sock)
        {
            if (send(clients[i], msg, strlen(msg), 0) < 0)
            {
                printf("Message sending failed\n");
            }
        }
    }
    pthread_mutex_unlock(&mutex);
}

void *handle_client(void *client_sock)
{
    int sock = *((int *)client_sock);
    char msg[BUFFER_SIZE];
    int len;

    while ((len = recv(sock, msg, BUFFER_SIZE, 0)) > 0)
    {
        msg[len] = '\0'; // Properly terminate the string
        send_to_all(msg, sock);
    }

    // If recv() returns <= 0, the client has disconnected
    printf("Client disconnected\n");
    close(sock);
    remove_client(sock);
    pthread_exit(NULL);
}

int main()
{
    struct sockaddr_in server_addr, client_addr;
    pthread_t thread;
    int server_sock, client_sock;
    socklen_t client_size = sizeof(client_addr);

    // Initialize mutex
    pthread_mutex_init(&mutex, NULL);

    // Create socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1)
    {
        perror("Socket creation failed");
        return 1;
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(1234);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Bind failed");
        return 1;
    }
    printf("Server started on port 1234\n");

    // Listen for clients
    if (listen(server_sock, MAX_CLIENTS) == -1)
    {
        perror("Listening failed");
        return 1;
    }

    while (1)
    {
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_size);
        if (client_sock < 0)
        {
            perror("Accept failed");
            continue;
        }

        pthread_mutex_lock(&mutex);
        if (n < MAX_CLIENTS)
        {
            clients[n++] = client_sock;
            pthread_create(&thread, NULL, handle_client, &client_sock);
            pthread_detach(thread); // No need to join, detach the thread
        }
        else
        {
            printf("Maximum clients reached. Connection rejected.\n");
            close(client_sock);
        }
        pthread_mutex_unlock(&mutex);
    }

    close(server_sock);
    pthread_mutex_destroy(&mutex);
    return 0;
}
