#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 6543

void stopwait(int client_sd)
{
    char frame[128], ack[32], seq = '0';
    int len;

    while (1)
    {
        // Receive frame from client
        len = read(client_sd, frame, sizeof(frame) - 1);
        if (len <= 0)
            break;
        frame[len] = '\0';

        printf("Received Frame: [%s]\n", frame);

        if (frame[0] == seq)
        {
            printf("Sequence Matched: %c\n", seq);
            snprintf(ack, sizeof(ack), "%cACK", seq);
            seq = (seq == '0') ? '1' : '0';
        }
        else
        {
            printf("Wrong Sequence: %c\n", seq);
            snprintf(ack, sizeof(ack), "%cNACK", seq);
        }

        // Send acknowledgment
        write(client_sd, ack, strlen(ack) + 1);
        printf("Sent Acknowledgment: [%s]\n\n", ack);

        if (strcmp(frame + 1, "EXIT") == 0)
            break;
    }
}

int main()
{
    int server_sd, client_sd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    printf("Starting TCP Server...\n");

    // Create socket
    if ((server_sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket creation failed");
        exit(1);
    }

    // Bind socket
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_sd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Binding failed");
        exit(1);
    }

    // Listen for connections
    listen(server_sd, 1);
    printf("Listening on port %d...\n", PORT);

    // Accept client connection
    client_sd = accept(server_sd, (struct sockaddr *)&client_addr, &client_len);
    if (client_sd < 0)
    {
        perror("Connection failed");
        exit(1);
    }
    printf("Client connected!\n");

    stopwait(client_sd);

    close(client_sd);
    close(server_sd);
    return 0;
}
