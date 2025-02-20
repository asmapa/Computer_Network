#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <ctype.h>

// Function to check if the string is a valid number
int is_valid_number(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) // If any character is not a digit
            return 0;
    }
    return 1;
}

// Function to check if a number is prime
int is_prime(int num) {
    if (num < 2)
        return 0;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0)
            return 0;
    }
    return 1;
}

// Function to get factors of a number
void get_factors(int num, char *buffer) {
    char temp[1024] = ""; // Temporary buffer to store factors
    strcpy(temp, "It is not prime. Factors: "); // Initial response

    for (int i = 1; i <= num; i++) {
        if (num % i == 0) {
            char factor[20];
            sprintf(factor, "%d ", i); // Convert factor to string
            strcat(temp, factor);      // Append factor to temp buffer
        }
    }

    strcpy(buffer, temp); // Copy final response to buffer
}

void main() {
    char *ip = "127.0.0.1"; // Server IP
    int port = 5566;        // Server port
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[1024];
    socklen_t addr_size;
    int bind_val;

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

    addr_size = sizeof(server_addr);

    // Binding socket to the address
    bind_val = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (bind_val < 0) {
        perror("[+] Bind error");
        exit(1);
    }

    printf("Server is running and waiting for messages...\n");

    while (1) {
        // Receive data from the client
        bzero(buffer, 1024);
        addr_size = sizeof(client_addr);
        recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr *)&client_addr, &addr_size);
        printf("[+] Data from client: %s\n", buffer);

        // Validate input
        if (!is_valid_number(buffer)) {
            strcpy(buffer, "Invalid input. Please send a valid number.");
        } else {
            int num = atoi(buffer);

            if (is_prime(num)) {
                strcpy(buffer, "It is prime.");
            } else {
                get_factors(num, buffer); // Get factors if not prime
            }
        }

        // Send response back to the client
        sendto(sockfd, buffer, strlen(buffer) + 1, 0, (struct sockaddr *)&client_addr, addr_size);
        printf("[+] Response sent to client: %s\n", buffer);
    }
}

