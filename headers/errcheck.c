#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "define.h"

void argument_check(int arg)
{
    if(arg < 2)
    {
        printf("Port argument missing!\n");
        exit(1);
    }
    else if(arg > 2)
    {
        printf("Too many arguments!\n");
    }
}

void retval_check(int retval, char *message)
{
    if(retval == SOCKET_ERROR)
    {
        perror(message);
        exit(1);
    }
}