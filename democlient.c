#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
    struct sockaddr_in server_addr, client_addr;
    int sock_desc, client_size;
    char buffer[1024];

    sock_desc = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

   
    client_size = sizeof(server_addr);
    while (1)
    {
        bzero(buffer, 1024);

        printf("ENter mesage to server:\n");
        fgets(buffer, sizeof(buffer), stdin);

        if (sendto(sock_desc, buffer, strlen(buffer),0, (struct sockaddr *)&server_addr, client_size) < 0)
        {
            printf("Error while sening data !!!\n");
            return 1;
        }

        if (recvfrom(sock_desc, buffer, sizeof(buffer), 0, (struct sockaddr *)&server_addr, &client_size) < 0)
        {
            printf("Error while receive message !!\n");
            return 1;
        }

        printf("client message : %s", buffer);
    }
}
