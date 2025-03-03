#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h> 

void capitalize(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        str[i] = toupper(str[i]);
    }
}

int main(void)
{
    int socket_desc, client_sock, client_size;
    struct sockaddr_in server_addr, client_addr;
    char client_message[2000];

    // Clean buffer
    memset(client_message, '\0', sizeof(client_message));

    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc < 0)
    {
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket created successfully\n");

    // Set port and IP
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);                   // Choose port
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Localhost

    // Bind the socket
    if (bind(socket_desc, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("Couldn't bind to the port\n");
        return -1;
    }
    printf("Done with binding\n");

    // Listen for clients
    if (listen(socket_desc, 1) < 0)
    {
        printf("Error while listening\n");
        return -1;
    }
    printf("\nListening for incoming connections.....\n");

    // Accept an incoming connection
    client_size = sizeof(client_addr);
    client_sock = accept(socket_desc, (struct sockaddr *)&client_addr, &client_size);
    if (client_sock < 0)
    {
        printf("Can't accept\n");
        return -1;
    }
    printf("Client connected at IP: %s and port: %i\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    // Handle client communication in a loop
    while (1)
    {
        // Receive message from client
        if (recv(client_sock, client_message, sizeof(client_message), 0) < 0)
        {
            printf("Couldn't receive\n");
            break;
        }
        printf("Msg from client: %s\n", client_message);

        // If client sends "bye", close connection
        if (strncmp(client_message, "bye", 3) == 0)
        {
            printf("Closing connection...\n");
            break;
        }

        // Capitalize the message
        capitalize(client_message);

        // Send back the capitalized message to the client
        if (send(client_sock, client_message, strlen(client_message), 0) < 0)
        {
            printf("Can't send\n");
            break;
        }

        // Clear buffer for the next iteration
        memset(client_message, '\0', sizeof(client_message));
    }

    // Close the sockets
    close(client_sock);
    close(socket_desc);

    return 0;
}
