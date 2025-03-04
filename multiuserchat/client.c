#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUFFER_SIZE 500
#define NAME_SIZE 100

char msg[BUFFER_SIZE];

void *receive_messages(void *socket_desc)
{
    int sock = *((int *)socket_desc);
    int len;
    while ((len = recv(sock, msg, BUFFER_SIZE - 1, 0)) > 0)
    {
        msg[len] = '\0'; // Properly terminate received message
        printf("%s", msg);
        fflush(stdout);
    }

    printf("Disconnected from server.\n");
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <client_name>\n", argv[0]);
        return 1;
    }

    pthread_t recv_thread;
    int sock;
    char send_msg[BUFFER_SIZE];
    struct sockaddr_in server_addr;
    char client_name[NAME_SIZE];

    strcpy(client_name, argv[1]);
    

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        perror("Socket creation failed");
        return 1;
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(1234);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Connection to server failed");
        close(sock);
        return 1;
    }

    // Create a thread to receive messages
    if (pthread_create(&recv_thread, NULL, receive_messages, &sock) != 0)
    {
        perror("Thread creation failed");
        close(sock);
        return 1;
    }

    // Read messages from console and send them
    while (fgets(msg, BUFFER_SIZE - 1, stdin) != NULL)
    {
        snprintf(send_msg, sizeof(send_msg), "%.99s: %.398s", client_name, msg);
        if (send(sock, send_msg, strlen(send_msg), 0) < 0)
        {
            perror("Message sending failed");
        }
    }

    // Close connection
    pthread_cancel(recv_thread);
    pthread_join(recv_thread, NULL);
    close(sock);
    return 0;
}
