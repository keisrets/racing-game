#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdbool.h>
#include "define.h"

void initialize_client(struct sockaddr_in *serverStruct, int port)
{
    memset(serverStruct, '\0', sizeof(struct sockaddr_in));
    serverStruct->sin_family = AF_INET;
    serverStruct->sin_port = htons(port);
    serverStruct->sin_addr.s_addr = inet_addr("127.0.0.1");
}

void initialize_server(struct sockaddr_in *serverStruct, int port)
{
    memset(serverStruct, '\0', sizeof(struct sockaddr_in));
    serverStruct->sin_family = AF_INET;
    serverStruct->sin_port = htons(port);
    serverStruct->sin_addr.s_addr = INADDR_ANY;
}

bool c_game_start(char *s_msg)
{
    if(strcmp(s_msg, START) == 0) return true;
    else return false;
}