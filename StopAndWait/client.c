#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 6543
#define SERVER_IP "127.0.0.1"

void stopwait(int server_sd)
{
    char frame[130], ack[32], seq = '0', input[128];

    while (1)
    {
        printf("Enter Message: ");
        scanf("%127[^\n]", input); 
        getchar();

        
        snprintf(frame, sizeof(frame), "%c%s", seq, input);

        write(server_sd, frame, strlen(frame) + 1);
        printf("Sent Frame: [%s]\n", frame);

        // Wait for acknowledgment
        read(server_sd, ack, sizeof(ack));
        printf("Received Acknowledgment: [%s]\n\n", ack);

        if (ack[1] == 'A')
        {
            seq = (seq == '0') ? '1' : '0';
        }
        else
        {
            printf("Resending Frame: [%s]\n", frame);
            continue;
        }

        if (strcmp(input, "EXIT") == 0)
            break;
    }
}

int main()
{
    int client_sd;
    struct sockaddr_in server_addr;

    printf("Starting TCP Client...\n");

    // Create socket
    if ((client_sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket creation failed");
        exit(1);
    }

    // Connect to server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(client_sd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Connection failed");
        exit(1);
    }
    printf("Connected to Server!\n");

    stopwait(client_sd);

    close(client_sd);
    return 0;
}