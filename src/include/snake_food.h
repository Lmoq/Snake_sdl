#ifndef SNAKE_H_H
#define SNAKE_H_H
#include "food.h"
#include "snake.h"
#include "main.h"

//   Snake
// Allocation and deallocation
void initSnake( Snakes *pSnake_, int p1Color );
void addSnake( Snakes *pSnake_ );
void growSnake( Snake *head, int size );
void growBody( Snake *body );
void deleteSnake( Snakes  *pSnake_ );

// Render
void drawSnakes( Snakes *pSnake_ );
void drawTrails( Snake *head, int color );

// Update
void checkSelfCol( Snake *head );
void chunkTurn( Snake *head );
void moveSnakes( Snakes *pSnake_, Food *food );
void moveBody( Snake *head );
void moveBodyTrails( Snake *body, Snake *head );
void moveTail( Snake *head );
void updateTail( Snake *head );


//  Food

void initFood( Food *food );
void generateFood( Food *food );
void drawFood( Food *food );

void checkFoodhasEaten( Snake *snake, Food *food );
void moveFood( Food *food );
void foodChunkTurn( Food *food );

void deleteFood( Food *food );


#endif