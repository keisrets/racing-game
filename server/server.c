// standard operation libs
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
// server libs
#include <math.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
// custom libs
#include "../headers/errcheck.h"
#include "../headers/common.h"
#include "../headers/game.h"
#include "../headers/packet.h"
#include "../headers/define.h"

// keep track of connections
int thread_count = 0;

// player 1 car variables
int p1_x = 1126;
int p1_y = 370;
float p1_angle = 0;
float p1_velocity = 0;

// player 2 car variables
int p2_x = 1031;
int p2_y = 370;
float p2_angle = 0;
float p2_velocity = 0;

// function prototypes
void *handle_traffic(struct thread_args *args);

// global server structs
struct sockaddr_in serverAddr, clientAddr;
socklen_t addr_size = sizeof(struct sockaddr_in);

int main(int argc, char **argv)
{
    // command line argument check
    argument_check(argc);

    int server_sockfd;
    int port = atoi(argv[1]);
    
    struct thread_args thread_sockets;
    // creating thread UDP socket...
    retval_check(thread_sockets.udp_sockfd = socket(AF_INET, SOCK_DGRAM, 0), "Could not create thread struct UDP socket.");

    // create server socket...
    retval_check(server_sockfd = socket(AF_INET, SOCK_STREAM, 0), "socket() error: ");

    // set server info...
    initialize_server(&serverAddr, port);

    // binding sockets to server address...
    retval_check(bind(server_sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)), "Could not bind thread struct TCP socket.");
    retval_check(bind(thread_sockets.udp_sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)), "Could not bind thread struct UDP socket.");

    // set to listen mode...
    retval_check(listen(server_sockfd, 2), "Listen error: ");
    printf("Waiting for connections...\n");
    
    // server logic...
    while(1)
    {
        // accept connection...
        retval_check(thread_sockets.c_tcp_sockfd = accept(server_sockfd, (struct sockaddr*)&clientAddr, (socklen_t*)&addr_size), "accept() error: ");
        printf("Connection accepted from %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
        
        // for every new client, create a new thread...
        pthread_t tID;
        thread_count++;
        pthread_create(&tID, NULL, (void*)handle_traffic, &thread_sockets);
    }
    return 0;
}

void *handle_traffic(struct thread_args *args)
{
    // add id to thread
    int thread_id = thread_count;
    char buffer[1024];
    while(1)
    {
        // if one client connected, start game (for now)
        if(thread_count == 1)
        {
            memset(&buffer, '\0', 1024);
            strcpy(buffer, START);
            retval_check(write(args->c_tcp_sockfd, buffer, 1023), "handleTraffic write() error: ");
            memset(&buffer, '\0', 1024);
            printf("Game started!\n");

            // Player 1
            if(thread_id == 1)
            {
                prepare_datagram(buffer, "P1|", p1_x, p1_y, p1_angle, "P2|", p2_x, p2_y, p2_angle);
                retval_check(write(args->c_tcp_sockfd, buffer, 1024), "Starting coordinate send error: ");
                memset(&buffer, '\0', 1024);

                retval_check(read(args->c_tcp_sockfd, buffer, 1024), "handleTraffic read() error: ");
                if(strcmp(buffer, START_COORD_OK) == 0)
                {
                    int lap_count = 0;
                    bool checkpoint_1 = false;
                    bool checkpoint_2 = false;
                    while(1)
                    {      
                        memset(&buffer, '\0', 1024);
                        retval_check(recvfrom(args->udp_sockfd, buffer, 1024, 0, (struct sockaddr*)&clientAddr, (socklen_t*)&addr_size), "receive err");
                        printf("%s\n", buffer);
                        if(strcmp(buffer, EXIT) == 0) exit(1);
                    
                        // received movement handling and calculations  
                        // arrow up
                        if(strcmp(buffer, RECV_UP) == 0)
                        {
                            if(p1_velocity < MAX_VELOCITY)
                            {
                                if(p1_velocity < 0) p1_velocity += DECELERATION;
                                else p1_velocity += ACCELERATION;
                            }
                        }
                        
                        // arrow down
                        if(strcmp(buffer, RECV_DOWN) == 0)
                        {
                            if(p1_velocity > -MAX_VELOCITY)
                            {
                                if(p1_velocity > 0)
                                {
                                    if(p1_velocity > -5)
                                    {
                                        p1_velocity -= DECELERATION;
                                    }
                                }
                                else p1_velocity -= ACCELERATION;
                            }
                        }
                        // not accelerating, not braking
                        if(strcmp(buffer, RECV_UP) != 0 && strcmp(buffer, RECV_DOWN) != 0)
                        {
                            if(p1_velocity - DECELERATION > 0)
                            {
                                p1_velocity -= DECELERATION;
                            } 
                            else if(p1_velocity + DECELERATION < 0)
                            {
                                p1_velocity += DECELERATION;
                            }
                            else
                            {
                                p1_velocity = 0;
                            } 
                        }
                        // turning calculations
                        // arrow left
                        if((strcmp(buffer, RECV_LEFT) == 0))
                        {
                            if(p1_velocity != 0)
                            {
                                p1_angle -= TURN_SPEED * p1_velocity/MAX_VELOCITY;
                            }
                        }
                        // if up/down and left pressed at the same time
                        if(strcmp(buffer, RECV_UP_LT) == 0)
                        {
                            if(p1_velocity < MAX_VELOCITY)
                            {
                                if(p1_velocity == 0)
                                {
                                    p1_velocity += 2*ACCELERATION;
                                }
                                if(p1_velocity < 6)
                                {
                                    p1_velocity += 2*ACCELERATION;
                                    p1_angle -= TURN_SPEED * (p1_velocity/MAX_VELOCITY);
                                }
                                else
                                {
                                    p1_angle -= TURN_SPEED * p1_velocity/MAX_VELOCITY;
                                }
                            }
                        }
                        if(strcmp(buffer, RECV_DOWN_LT) == 0)
                        {
                            if(p1_velocity < MAX_VELOCITY)
                            {
                                if(p1_velocity == 0)
                                {
                                    p1_velocity -= DECELERATION;
                                }
                                if(p1_velocity > -3)
                                {
                                    p1_velocity -= DECELERATION;
                                    p1_angle -= TURN_SPEED * (p1_velocity/MAX_VELOCITY);
                                }
                                else
                                {
                                    p1_angle -= TURN_SPEED * p1_velocity/MAX_VELOCITY;
                                }
                            }
                        }
                        // arrow right
                        if((strcmp(buffer, RECV_RIGHT) == 0))
                        {
                            if(p1_velocity != 0)
                            {
                                p1_angle += TURN_SPEED * p1_velocity/MAX_VELOCITY;
                            }
                        }
                        // if up/down and right pressed at the same time
                        if(strcmp(buffer, RECV_UP_RT) == 0)
                        {
                            if(p1_velocity < MAX_VELOCITY)
                            {
                                if(p1_velocity == 0)
                                {
                                    p1_velocity += 2*ACCELERATION;
                                }
                                if(p1_velocity < 6)
                                {
                                    p1_velocity += 2*ACCELERATION;
                                    p1_angle += TURN_SPEED * (p1_velocity/MAX_VELOCITY);
                                }
                                else
                                {
                                    p1_angle += TURN_SPEED * p1_velocity/MAX_VELOCITY;
                                }
                            }
                        }
                        if(strcmp(buffer, RECV_DOWN_RT) == 0)
                        {
                            if(p1_velocity < MAX_VELOCITY)
                            {
                                if(p1_velocity == 0)
                                {
                                    p1_velocity -= DECELERATION;
                                }
                                if(p1_velocity > -3)
                                {
                                    p1_velocity -= DECELERATION;
                                    p1_angle += TURN_SPEED * (p1_velocity/MAX_VELOCITY);
                                }
                                else
                                {
                                    p1_angle += TURN_SPEED * p1_velocity/MAX_VELOCITY;
                                }
                            }
                        }
                        // collision with side walls
                        if(p1_x <= 80)
                        {
                            p1_x = 85;
                            p1_velocity = 0;
                        }
                        else if(p1_x >= 1159)
                        {
                            p1_x = 1154;
                            p1_velocity = 0;
                        }
                        // collision with top and bottom wall
                        else if(p1_y <= 80)
                        {
                            p1_y = 85;
                            p1_velocity = 0;
                        }
                        else if(p1_y >= 550)
                        {
                            p1_y = 545;
                            p1_velocity = 0;
                        }

                        // collision with center wall
                        if((p1_x >= 300 && p1_x <= 910))
                        {
                            if(p1_y >= 245 && p1_y < 260)
                            {
                                p1_y = 240;
                                p1_velocity = 0;
                            }
                            if(p1_y <= 380 && p1_y > 370)
                            {
                                p1_y = 385;
                                p1_velocity = 0;
                            }
                        }

                        // checkpoints
                        if((p1_x == 650 && (p1_y >= 65 && p1_y <= 250)))
                        {
                            if(!checkpoint_1) checkpoint_1 = true;
                        }
                        if((p1_x == 650 && (p1_y >= 380 && p1_y <= 570)))
                        {
                            if(!checkpoint_2) checkpoint_2 = true;
                        }
                        //if finish line crossed and checkpoints visited, increment lap count
                        if(p1_y == 315 && (p1_x <= 1150 && p1_x >= 990))
                        {
                            if(lap_count == 2)
                            {
                                break;
                            }
                            if(checkpoint_1 && checkpoint_2)
                            {
                                lap_count++;
                                checkpoint_1 = checkpoint_2 = false;
                            }
                        }
                        // setting new coordinates
                        p1_x += sin(p1_angle) * p1_velocity;
                        p1_y -= cos(p1_angle) * p1_velocity;
                                
                        memset(&buffer, '\0', 1024);
                        // prepare datagram
                        prepare_datagram(buffer, "P1|", p1_x, p1_y, p1_angle, "P2|", p2_x, p2_y, p2_angle);
                        // send datagram to client
                        retval_check(sendto(args->udp_sockfd, buffer, 1024, 0, (struct sockaddr*)&clientAddr, addr_size), "second receive err");
                    }
                } 
            }
            /*
            // Player 2
            if(thread_id == 2)
                {
                    // send initial coordinates to player 2
                    
                    {
                        while(1)
                        {   
                                under construction...      
                        }
                    }
                    
                }
                */
        }
        else 
        {
            // if not enough players for game, let client know
            memset(&buffer, '\0', 1024);
            strcpy(buffer, "waiting for more players...");
            retval_check(write(args->c_tcp_sockfd, buffer, 1023), "handleTraffic write() error: ");
            memset(&buffer, '\0', 1024);
        }
    }
    return NULL;
}