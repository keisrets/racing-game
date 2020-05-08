// standard operation libs
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
// client libs
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netdb.h>
// custom libs
#include "../headers/errcheck.h"
#include "../headers/common.h"
#include "../headers/game.h"
#include "../headers/define.h"
#include "../headers/packet.h"

int main(int argc, char **argv)
{
    // check command line arguments...
    argument_check(argc);

    // initialise helper variables...
    struct sockaddr_in serverAddr;
    socklen_t addr_size = sizeof(struct sockaddr_in);
    char buffer[1024];
    int tcp_sock, udp_sock;
    int port = atoi(argv[1]);
    char player_ID[2];
    memset(player_ID, '\0', sizeof(player_ID));
    //strcpy(player_ID, "P1");

    // create socket
    retval_check(tcp_sock = socket(AF_INET, SOCK_STREAM, 0), "socket() error: ");
    retval_check(udp_sock = socket(AF_INET, SOCK_DGRAM, 0), "UDP socket creation failed: ");

    // set server info...
    //initClient(&serverAddr, port);
    initialize_client(&serverAddr, port);

    // connect to server...
    retval_check(connect(tcp_sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)), "connect() error: ");
    memset(buffer, '\0', 1024);
    // after connection successful...
    while(1)
    {
        // null buffer...
        memset(buffer, '\0', 1024);
        strcpy(buffer, "MSG_READY");
        // tell server that player is ready
        retval_check(write(tcp_sock, buffer, strlen(buffer)), "write() error: ");
        memset(buffer, '\0', 1024);
        // get confirmation from server
        retval_check(read(tcp_sock, buffer, 1024), "read() error: ");
        if(c_game_start(buffer))
        {
            // defining game main variables
            int p1_x, p1_y, p2_x, p2_y;
            float p1_angle = 0, p2_angle = 0;
            printf("Game started!\n");
    
            // get self coordinates...
            retval_check(read(tcp_sock, buffer, 1023), "read() error: ");
            // handle message...
            unpack_initial_coordinates(buffer, player_ID, &p1_x, &p1_y, &p1_angle, &p2_x, &p2_y, &p2_angle);
            memset(buffer, '\0', 1024);

            // if received self coordinates successfully, let server know...
            retval_check(write(tcp_sock, "R_COORDINATES", 1024), "read() error: ");
            memset(buffer, '\0', 1024);

            // checking if correct
            printf("My coordinates: x=%d, y=%d\n", p1_x, p1_y);
            printf("Rival coordinates: x=%d, y=%d\n", p2_x, p2_y);

            // building game...
            SDL_Window *g_window;
            SDL_Renderer *g_renderer;
            SDL_Texture *t_pcar, *t_rcar, *t_twall, *t_swall, *t_cwall, *t_finish;
            SDL_Rect r_pcar, r_rcar, r_twall, r_lwall, r_lfwall, r_rtwall, r_cwall, r_finish;

            // initialize game window...
            initialize_game_window(&g_window, &g_renderer);
            // create game textures
            create_game_textures(&g_window, &g_renderer, &t_pcar, &t_rcar, &t_twall, &t_swall, &t_cwall, &t_finish);
            // add textures to game window
            SDL_QueryTexture(t_pcar, NULL, NULL, &r_pcar.w, &r_pcar.h);
            SDL_QueryTexture(t_rcar, NULL, NULL, &r_rcar.w, &r_rcar.h);
            SDL_QueryTexture(t_twall, NULL, NULL, &r_twall.w, &r_twall.h);
            SDL_QueryTexture(t_twall, NULL, NULL, &r_lwall.w, &r_lwall.h);
            SDL_QueryTexture(t_swall, NULL, NULL, &r_lfwall.w, &r_lfwall.h);
            SDL_QueryTexture(t_swall, NULL, NULL, &r_rtwall.w, &r_rtwall.h);
            SDL_QueryTexture(t_cwall, NULL, NULL, &r_cwall.w, &r_cwall.h);
            SDL_QueryTexture(t_finish, NULL, NULL, &r_finish.w, &r_finish.h);
            printf("querying successfull!\n");

            // initial car and object scaling and positioning...
            set_car_start_pos(&r_pcar, &r_rcar, p1_x, p1_y, p2_x, p2_y);
            set_object_values(&r_twall, &r_lwall, &r_lfwall, &r_rtwall, &r_cwall, &r_finish);

            // key event handling...
            bool close_event = false;
            int up, down, left, right;
            up = down = left = right = 0;
            while(!close_event)
            {
                SDL_Event control;
                while(SDL_PollEvent(&control))
                {
                    memset(&buffer, '\0', 1024);
                    switch (control.type)
                    {
                    case SDL_QUIT:
                    close_event = true;
                    break;
                    case SDL_KEYDOWN:
                    switch(control.key.keysym.scancode)
                    {
                        case SDL_SCANCODE_UP:
                            up = 1;
                            break;
                        
                        case SDL_SCANCODE_DOWN:
                            down = 1;
                            break;
                        
                        case SDL_SCANCODE_LEFT:
                            left = 1;
                            break;
                        
                        case SDL_SCANCODE_RIGHT:
                            right = 1;
                            break;
                        }
                    break;
                    case SDL_KEYUP:
                        switch(control.key.keysym.scancode)
                        {
                        case SDL_SCANCODE_UP:
                            up = 0;
                            break;
                        
                        case SDL_SCANCODE_DOWN:
                            down = 0;
                            break;
                        
                        case SDL_SCANCODE_LEFT:
                            left = 0;
                            break;
                        
                        case SDL_SCANCODE_RIGHT:
                            right = 0;
                            break;
                        }
                    break;
                    }
                }
                strcpy(buffer, player_ID);
                strcat(buffer, "|");
                if(up)
                {
                    if(left)
                    {
                        strcat(buffer, UP_LEFT);
                    }
                    else if(right)
                    {
                        strcat(buffer, UP_RIGHT);
                    }
                    else
                    {
                        strcat(buffer, UP);
                    }
                }
                else if(down)
                {
                    if(left)
                    {
                        strcat(buffer, DOWN_LEFT);
                    }
                    else if(right)
                    {
                        strcat(buffer, DOWN_RIGHT);
                    }
                    else
                    {
                        strcat(buffer, DOWN);
                    }
                }
                else if(left)
                {
                    strcat(buffer, LEFT);
                }
                else if(right)
                {
                    strcat(buffer, RIGHT);
                }
                
                // send key event...
                retval_check(sendto(udp_sock, buffer, 1024, 0, (struct sockaddr*)&serverAddr, addr_size) , "key event send failed: ");
                memset(&buffer, '\0', 1024);
                // receive car coordinates..
                retval_check(recvfrom(udp_sock, buffer, 1024, 0, (struct sockaddr*)&serverAddr, (socklen_t*)&addr_size) , "key event send failed: ");
                printf("%s\n", buffer);
                //retval_check(recvfrom(udp_sock, buffer, 1024, 0, (struct sockaddr*)&serverAddr, (socklen_t*)&addr_size) , "key event send failed: ");
                unpack_datagram(buffer, &p1_x, &p1_y, &p1_angle, &p2_x, &p2_y, &p2_angle);
                r_pcar.x = p1_x;
                r_pcar.y = p1_y;
                //r_rcar.x = p2_x;
                //r_rcar.y = p2_y;
                // clear screen and copy objects to render
                render_screen(&g_renderer, &t_twall, &t_swall, &t_cwall, &t_finish, &r_twall, &r_lfwall, &r_rtwall, &r_lwall, &r_cwall, &r_finish);
                // copy cars to render
                SDL_RenderCopyEx(g_renderer, t_pcar, NULL, &r_pcar, p1_angle*180/3.141592, NULL, SDL_FLIP_VERTICAL);
                //SDL_RenderCopyEx(g_renderer, t_rcar, NULL, &r_rcar, p2_angle*180/3.141592, NULL, SDL_FLIP_VERTICAL);
                // show render on window
                SDL_RenderPresent(g_renderer);
                SDL_Delay(1000/60);
                
                memset(&buffer, '\0', 1024);
            }
            strcpy(buffer, "P1|CMD_EXIT");
            retval_check(sendto(udp_sock, buffer, 1024, 0, (struct sockaddr*)&serverAddr, addr_size) , "key event send failed: ");
            // destroy game variables and close window
            game_destroy(&g_window, &g_renderer, &t_twall, &t_swall, &t_cwall, &t_pcar, &t_rcar, &t_finish);  
            break;   
        }
        else
        {
            continue;
        }
    }
}