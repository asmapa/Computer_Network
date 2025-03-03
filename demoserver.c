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
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_size;

    bzero(buffer, sizeof(buffer));

    // Create socket
    sock_desc = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_desc < 0)
    {
        perror("[-] Socket error");
        exit(1);
    }

    // Configure server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Bind
    if (bind(sock_desc, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("Couldn't bind to the port\n");
        return -1;
    }

    printf("Binding successful, server is ready!\n");
    client_size = sizeof(client_addr);

    while (1)
    {
        // Receive number from client
        if (recvfrom(sock_desc, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &client_size) < 0)
        {
            printf("Error while receiving");
            break;
        }
        printf("Received data: %s\n", buffer);

        int num = atoi(buffer);
        if (num == 1 || num == 2)
        {
            strcpy(buffer, "It is prime.");
        }
        else
        {
            int flag = 0;
            for (int i = 2; i <= num / 2; i++)
            {
                if (num % i == 0)
                {
                    flag = 1;
                    break;
                }
            }

            if (flag == 0)
            {
                strcpy(buffer, "It is prime.");
            }
            else
            {
                strcpy(buffer, "It is not a prime. Factors: ");
                for (int i = 1; i <= num / 2; i++)
                { // FIXED: i should start from 1
                    if (num % i == 0)
                    {
                        char fact[20];
                        sprintf(fact, "%d ", i);
                        strcat(buffer, fact);
                    }
                }
            }
        }

        // Send response to client
        if (sendto(sock_desc, buffer, strlen(buffer), 0, (struct sockaddr *)&client_addr, client_size) < 0)
        {
            printf("Error while sending");
            break;
        }
    }

    close(sock_desc);
    return 0;
}
