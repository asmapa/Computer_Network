#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main()
{
    char buffer[1024];
    int sock_desc;
    struct sockaddr_in server_addr;
    socklen_t server_size;

    bzero(buffer, sizeof(buffer));

    // Create socket
    sock_desc = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_desc < 0)
    {
        perror("[-] Socket error");
        exit(1);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Client ready!\n");
    server_size = sizeof(server_addr);

    while (1)
    {
        printf("Enter a number to check for prime: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // FIXED: Remove newline character

        // Send number to server
        if (sendto(sock_desc, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, server_size) < 0)
        {
            printf("Error while sending");
            break;
        }

        // Receive server response
        if (recvfrom(sock_desc, buffer, sizeof(buffer), 0, (struct sockaddr *)&server_addr, &server_size) < 0)
        {
            printf("Error while receiving");
            break;
        }

        printf("Server response: %s\n", buffer);
    }

    close(sock_desc);
    return 0;
}
