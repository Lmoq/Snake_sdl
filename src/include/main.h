#ifndef MAIN_H
#define MAIN_H
#include <stdbool.h>
#include <SDL2/SDL.h>

#define SIZE 20
#define FPS 100
#define FRAME_TIME (float)1000 / FPS
#define WIDTH SIZE * 60
#define HEIGHT SIZE * 35

#define BLUE 200
#define GREEN 201
#define RED 202
#define ORANGE 203

SDL_Event event;
SDL_KeyCode keycode;
SDL_Renderer *renderer;
SDL_Window *gameWindow;

bool running;
bool menu;
bool foodAlive;

int last_frame_time;
int time_to_wait;
int playerNum;

float delta_time;

void setup();
void listen();
void update();
void render();
void terminate();

#endif