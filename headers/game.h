#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>


// game object creation and window rendering functions
void initialize_game_window(SDL_Window **window, SDL_Renderer **render);
void create_game_textures(SDL_Window **window, SDL_Renderer **render, SDL_Texture **p_car, SDL_Texture **r_car, SDL_Texture **topWall, SDL_Texture **sideWall, SDL_Texture **centerWall, SDL_Texture **finishLine);
void set_object_values(SDL_Rect *top, SDL_Rect *bottom, SDL_Rect *left, SDL_Rect *right, SDL_Rect *center, SDL_Rect *finish);
void set_car_start_pos(SDL_Rect *pcar, SDL_Rect *rcar, int p1x, int p1y, int p2x, int p2y);
void render_screen(SDL_Renderer **render, SDL_Texture **top_wall, SDL_Texture **side_wall, SDL_Texture **center_wall, SDL_Texture **finish_line, SDL_Rect *r_top, SDL_Rect *r_left, SDL_Rect *r_right, SDL_Rect *r_bottom, SDL_Rect *r_center, SDL_Rect *r_finish);

// delete game variables after game end
void game_destroy(SDL_Window **window, SDL_Renderer **render, SDL_Texture **top_wall, SDL_Texture **side_wall, SDL_Texture **center_wall, SDL_Texture **p_car, SDL_Texture **r_car, SDL_Texture **finish_line);