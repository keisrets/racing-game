#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

void unpack_initial_coordinates(char *s_msg, char p_id[], int *p1_x, int *p1_y, float *p1_angle, int *p2_x, int *p2_y, float *p2_angle)
{
    // set player 1 coordinates
    char *id = strtok(s_msg, "|");
    strcpy(p_id, id);
    char *num1 = strtok(NULL, ":");
    *p1_x = atoi(num1);
    char *num2 = strtok(NULL, "/");
    *p1_y = atoi(num2);
    char *num3 = strtok(NULL, "+");
    *p1_angle = strtof(num3, NULL);
    
    // set player 2 coordinates
    char *id2 = strtok(NULL, "|");
    //strcpy(r_id, id2);
    char *_num1 = strtok(NULL, ":");
    *p2_x = atoi(_num1);
    char *_num2 = strtok(NULL, "/");
    *p2_y = atoi(_num2);
    char *_num3 = strtok(NULL, " ");
    *p2_angle = strtof(_num3, NULL);
}

void prepare_datagram(char buffer[], char *id1, int x1, int y1, float a1, char *id2, int x2, int y2, float a2)
{
    // cast player 1 variables to strings
    char *x1_buff = malloc(10);
    sprintf(x1_buff, "%d", x1);
    char *y1_buff = malloc(10);
    sprintf(y1_buff, "%d", y1);
    char *a1_buff = malloc(12);
    sprintf(a1_buff, "%f", a1);

    // cast player 2 variables to strings
    char *x2_buff = malloc(10);
    sprintf(x2_buff, "%d", x2);
    char *y2_buff = malloc(10);
    sprintf(y2_buff, "%d", y2);
    char *a2_buff = malloc(12);
    sprintf(a2_buff, "%f", a2);

    // add strings to buffer
    strcpy(buffer, id1);
    strcat(buffer, x1_buff);
    strcat(buffer, ":");
    strcat(buffer, y1_buff);
    strcat(buffer, "/");
    strcat(buffer, a1_buff);
    strcat(buffer, "+");
    strcat(buffer, id2);
    strcat(buffer, x2_buff);
    strcat(buffer, ":");
    strcat(buffer, y2_buff);
    strcat(buffer, "/");
    strcat(buffer, a2_buff);

    // free allocated strings
    free(x1_buff);
    free(y1_buff);
    free(a1_buff);
    free(x2_buff);
    free(y2_buff);
    free(a2_buff);
}

void unpack_datagram(char *s_msg, int *p1_x, int *p1_y, float *p1_a, int *p2_x, int *p2_y, float *p2_a)
{
    // set Player 1 coordinates
    char *id1 = strtok(s_msg, "|");
    char *x1_num = strtok(NULL, ":");
    *p1_x = atoi(x1_num);
    char *y1_num = strtok(NULL, "/");
    *p1_y = atoi(y1_num);
    char *a1_num = strtok(NULL, "+");
    *p1_a = strtof(a1_num, NULL);
    
    // set Player 2 coordinates
    char *id2 = strtok(NULL, "|");
    char *x2_num = strtok(NULL, ":");
    *p2_x = atoi(x2_num);
    char *y2_num = strtok(NULL, "/");
    *p2_y = atoi(y2_num);
    char *a2_num = strtok(NULL, " ");
    *p2_a = strtof(a2_num, NULL);
}