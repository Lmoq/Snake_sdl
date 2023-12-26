#ifndef SNAKE_H
#define SNAKE_H
#include <stdbool.h>
#include <SDL2/SDL.h>

#define LEFT 101
#define RIGHT 102
#define UP 103
#define DOWN 104
#define STARTSIZE 2

typedef struct Snake {
    int SPEED;
    int DIRECTION;
    int BUFFDIR;
    int COLOR;

    // Previous directions
    int LASTDIR;
    int LASTLASTDIR;
    
    SDL_Point pointHead;
    SDL_Rect rect;

    bool snakeDead;

    // Body will grow then tail is attached
    struct Snake *body;
    struct Snake *bodyEnd;
    struct Snake *tail;

    // Extends to any number of snakes
    struct Snake *nextHead;
} Snake;


typedef struct Snakes {
    int numSnakes;  // player count
    Snake *snake;   // player list
} Snakes;
Snakes *pSnake;

SDL_Point STARTPOS1;
SDL_Point STARTPOS2;
SDL_Point STARTPOS3;
SDL_Point STARTPOS4;


#endif


