#ifndef SNAKE_H_H
#define SNAKE_H_H
#include "food.h"
#include "snake.h"
#include "main.h"

// Updates head LASTLASTDIR only after passing a chunk every turn to secure turns
#define updateTrailsHeadLastdir( body, head, prevBody) \
do { \
    moveBodyTrails( body, head ); \
    updateTail( head ); \
    body->LASTDIR = prevBody->LASTDIR; \
    head->LASTLASTDIR = head->LASTDIR; \
} while (0)

// Update lastdir for tail to follow bodyEnd.lastDir before bodyEnd makes a turn
#define followTrails( body, head, prevBody ) \
do { \
    moveBodyTrails(body, head); \
    body->LASTDIR = prevBody->LASTDIR; \
} while (0)


//   Snake
// Allocation and deallocation
Snakes *initSnake();
Snake *addSnake( int snakeIndex );
void growSnake( Snake *head, int size );
void growBody( Snake *body );
void setSnakePos( Snake *head , int snakeIndex);
void setupKeys();

void deleteHead( Snake *head );
void deleteSnakes( Snakes  *pSnake_ );

// Render
void drawSnakes( Snakes *pSnake_ );
void drawTrails( Snake *head, int color );

// Update
void checkSnakeCol( Snake *head , Snake *targetHead);
void checkOtherCol( Snake *head , Snakes *snakelist);
void chunkTurn( Snake *head );
void moveSnakes( Snakes *pSnake_, Food *food );
void moveBody( Snake *head );
void moveBodyTrails( Snake *body, Snake *head );
void moveTail( Snake *head );
void respawnSnake( Snake **head );
void updateTail( Snake *head );
void wrapAroundMap( Snake *head );


//  Food
void initFood( Food *food );
void generateFood( Food *food, Snakes *snakelist );
void drawFood( Food *food );

void checkFoodhasEaten( Snake *snake, Food *food );
void moveFood( Food *food );
void foodChunkTurn( Food *food );

void deleteFood( Food *food );


#endif