#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <ctype.h>

void main()
{
    char *ip = "127.0.0.1"; // Server IP
    int port = 5566;        // Server port
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[1024];
    socklen_t addr_size;
    int bind_val;

    // Creating socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("[-] Socket error");
        exit(1);
    }

    // Server address initialization
    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    addr_size = sizeof(server_addr);

    // Binding socket to the address
    bind_val = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (bind_val < 0)
    {
        perror("[+] Bind error");
        exit(1);
    }

    printf("Server is running and waiting for messages...\n");

    // Continuous communication loop
    while (1)
    {
        // Receive data from the client
        bzero(buffer, 1024);
        addr_size = sizeof(client_addr); // Initialize the client address size
        recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr *)&client_addr, &addr_size);
        printf("[+] Data from client: %s\n", buffer);

        // Send response back to the client
        printf("Enter message to send to client: ");
        fgets(buffer, 1024, stdin);           // Input message to send back
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove trailing newline

        int bytes_sent = sendto(sockfd, buffer, 1024, 0, (struct sockaddr *)&client_addr, addr_size);
        printf("Number of bytes sent : %d \n", bytes_sent);
    }
}
