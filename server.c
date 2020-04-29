#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>

#define PORT 440

int main(void)
{
    int sockfd, ret;
    struct sockaddr_in serverAddr;

    int newSocket;
    struct sockaddr_in newAddr;
    char buffer[1024];
    socklen_t addr_size;
    pid_t child;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    {
        printf("Server socket error...\n");
        exit(1);
    }
    printf("Server socket created successfully!\n");

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (ret < 0)
    {
        printf("Socket binding error... \nReturned %d\n", ret);
        perror("\n");
        exit(1);
    }
    printf("Bind to port %d successfully\n", PORT);
    if(listen(sockfd, 1) == 0)
    {
        printf("Listening...\n");
    }
    else 
    {
        printf("Listening error...\n");
        exit(1);
    }

    while(1)
    {
        newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
        if (newSocket < 0)
        {
            exit(1);
        }
        printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
    
        if((child = fork()) == 0)
        {
            close(sockfd);

            while(1)
            {
                recv(newSocket, buffer, 1024, 0);
                if (strcmp(buffer, ":exit") == 0)
                {
                    printf("%s:%d disconnected from server\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
                    break;
                }
                else 
                {
                    printf("Client message: %s\n", buffer);
                    send(newSocket, buffer, 1024, 0);
                    memset(&buffer, '\0', sizeof(buffer));
                }
            }
        }
    }
    close(newSocket);

    return 0;
}
