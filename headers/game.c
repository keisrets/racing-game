#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include "define.h"

void initialize_game_window(SDL_Window **window, SDL_Renderer **render)
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        printf("Error initializing window: %s\n", SDL_GetError());
        exit(1);
    }
    printf("Initialization successful\n");

    *window = SDL_CreateWindow("Game window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if(!*window)
    {
        printf("Could not create the window: %s\n", SDL_GetError());
        exit(1);
    }

    printf("Window creation successful!\n");

    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    *render = SDL_CreateRenderer(*window, -1, render_flags);
    if(!*render)
    {
        printf("Could not create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        SDL_Quit();
        exit(1);
    }
}

void create_game_textures(SDL_Window **window, SDL_Renderer **render, SDL_Texture **p_car, SDL_Texture **r_car, SDL_Texture **topWall, SDL_Texture **sideWall, SDL_Texture **centerWall, SDL_Texture **finishLine)
{
    SDL_Surface *s_pcar, *r_rcar, *s_topWall, *s_sideWall, *s_centerWall, *s_finishLine;

    s_pcar = IMG_Load("client/game/sprites/p_car.png");
    r_rcar = IMG_Load("client/game/sprites/r_car.png");
    s_topWall = IMG_Load("client/game/sprites/outer_wall.png");
    s_sideWall = IMG_Load("client/game/sprites/side_wall.png");
    s_centerWall = IMG_Load("client/game/sprites/center_wall.png");
    s_finishLine = IMG_Load("client/game/sprites/finish_line.png");

    if(!s_pcar || !r_rcar || !s_topWall || !s_sideWall || !s_centerWall || !s_finishLine)
    {
        printf("Could not create surface: %s\n", SDL_GetError());
        SDL_DestroyRenderer(*render);
        SDL_DestroyWindow(*window);
        SDL_Quit();
        exit(1);
    }
    *p_car = SDL_CreateTextureFromSurface(*render, s_pcar);
    *r_car = SDL_CreateTextureFromSurface(*render, r_rcar);
    *topWall = SDL_CreateTextureFromSurface(*render, s_topWall);
    *sideWall = SDL_CreateTextureFromSurface(*render, s_sideWall);
    *centerWall = SDL_CreateTextureFromSurface(*render, s_centerWall);
    *finishLine = SDL_CreateTextureFromSurface(*render, s_finishLine);

    SDL_FreeSurface(s_pcar);
    SDL_FreeSurface(r_rcar);
    SDL_FreeSurface(s_topWall);
    SDL_FreeSurface(s_sideWall);
    SDL_FreeSurface(s_centerWall);
    SDL_FreeSurface(s_finishLine);

    if(!*p_car || !*r_car || !*topWall || !*sideWall || !*centerWall || !*finishLine)
    {
        printf("Could not create texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(*render);
        SDL_DestroyWindow(*window);
        SDL_Quit();
        exit(1);
    }
    printf("Textures created successfully!\n");
}

void set_object_values(SDL_Rect *top, SDL_Rect *bottom, SDL_Rect *left, SDL_Rect *right, SDL_Rect *center, SDL_Rect *finish)
{
    // wall properties
    top->w = WINDOW_WIDTH;
    top->h = 80;
    top->x = 0;
    top->y = 0;

    bottom->w = WINDOW_WIDTH;
    bottom->h = 80;
    bottom->x = 0;
    bottom->y = 640;

    left->w = 80;
    left->h = WINDOW_HEIGHT;
    left->x = 0;
    left->y = 0;

    right->w = 80;
    right->h = WINDOW_HEIGHT;
    right->x = 1200;
    right->y = 0;

    center->w = 630;
    center->h = 30;
    center->x = (WINDOW_WIDTH - 630) / 2;
    center->y = (WINDOW_HEIGHT - 30) / 2;

    finish->x = 950;
    finish->y = (WINDOW_HEIGHT - finish->h) / 2;
}

void set_car_start_pos(SDL_Rect *pcar, SDL_Rect *rcar, int p1x, int p1y, int p2x, int p2y)
{
    // car properties
    pcar->w = 40;
    pcar->h = 90;
    pcar->x = p1x;
    pcar->y = p1y;

    rcar->w = 40;
    rcar->h = 90;
    rcar->x = p2x;
    rcar->y = p2y;
}

void render_screen(SDL_Renderer **render, SDL_Texture **top_wall, SDL_Texture **side_wall, SDL_Texture **center_wall, SDL_Texture **finish_line, SDL_Rect *r_top, SDL_Rect *r_left, SDL_Rect *r_right, SDL_Rect *r_bottom, SDL_Rect *r_center, SDL_Rect *r_finish)
{
    SDL_RenderClear(*render);
    SDL_RenderCopy(*render, *top_wall, NULL, r_top);
    SDL_RenderCopy(*render, *side_wall, NULL, r_left);
    SDL_RenderCopy(*render, *side_wall, NULL, r_right);
    SDL_RenderCopy(*render, *top_wall, NULL, r_bottom);
    SDL_RenderCopy(*render, *center_wall, NULL, r_center);
    SDL_RenderCopy(*render, *finish_line, NULL, r_finish);
}

void game_destroy(SDL_Window **window, SDL_Renderer **render, SDL_Texture **top_wall, SDL_Texture **side_wall, SDL_Texture **center_wall, SDL_Texture **p_car, SDL_Texture **r_car, SDL_Texture **finish_line)
{
    SDL_DestroyTexture(*top_wall);
    SDL_DestroyTexture(*side_wall);
    SDL_DestroyTexture(*center_wall);
    SDL_DestroyTexture(*p_car);
    SDL_DestroyTexture(*finish_line);
    SDL_DestroyRenderer(*render);
    SDL_DestroyWindow(*window);
    SDL_Quit();  
}