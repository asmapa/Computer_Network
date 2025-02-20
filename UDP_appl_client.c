#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void main() {
    char *ip = "127.0.0.1"; // Server IP
    int port = 5566;        // Server port
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[1024];
    socklen_t addr_size;

    // Creating socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("[-] Socket error");
        exit(1);
    }

    // Server address initialization
    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    printf("Enter a number: ");
    fgets(buffer, 1024, stdin);
    buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline

    // Send number to server
    sendto(sockfd, buffer, strlen(buffer) + 1, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // Receive response from server
    addr_size = sizeof(server_addr);
    recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr *)&server_addr, &addr_size);
    printf("Server: %s\n", buffer);
}

