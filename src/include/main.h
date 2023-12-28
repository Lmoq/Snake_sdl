#ifndef MAIN_H
#define MAIN_H
#include <stdbool.h>
#include <SDL2/SDL.h>

#define SIZE 20
#define FPS 100
#define FRAME_TIME (float)1000 / FPS
#define WIDTH SIZE * 60
#define HEIGHT SIZE * 35
#define PSPEED SIZE * 15
#define FSPEED PSPEED

#define BLUE 200
#define GREEN 201
#define RED 202
#define ORANGE 203
#define WHITE 204
#define PALEWHITE 205
#define BLACK 206

#define setRenderColorBLUE  do { SDL_SetRenderDrawColor( renderer, 0, 0, 255, 255 ); } while (0)
#define setRenderColorRED do { SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 ); } while (0)
#define setRenderColorORANGE do { SDL_SetRenderDrawColor( renderer, 255, 165, 0, 255 ); } while(0)
#define setRenderColorGREEN do { SDL_SetRenderDrawColor( renderer, 0, 255, 0, 255 ); } while(0)
#define setRenderColorWHITE do { SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 ); } while (0)
#define setRenderColorPALEWHITE do { SDL_SetRenderDrawColor( renderer, 200, 200, 200, 255 ); } while (0)
#define setRenderColorBLACK do { SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 ); } while (0)

SDL_Event event;
SDL_KeyCode keycode;
SDL_Renderer *renderer;
SDL_Window *gameWindow;

bool running;
bool menu;

int last_frame_time;
int time_to_wait;
int playerNum;

float delta_time;

void setRenderColor( int COLOR );

void setup();
void listen();
void update();
void render();
void terminate();

#endif