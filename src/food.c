#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>
#include "main.h"
#include "food.h"
#include "snake.h"
#include "snake_food.h"


void initFood( Food *food ) {
    srand(time(NULL));
    if ( !food ) {
        return;
    }

    food->rect.w = SIZE;
    food->rect.h = SIZE;
    generateFood( food, SnakeList);

    // Set food movement
    food->FoodDIR = DOWN;
    food->FoodBuffDir = LEFT;

    food->FoodSPEED = FSPEED;
}

void drawFood( Food *food ) {
    // Render
    if ( !food ) {
        return;
    }
    SDL_SetRenderDrawColor( renderer, 0, 230, 0, 255 );
    SDL_RenderDrawRect( renderer, &food->rect );
    // SDL_RenderFillRect( renderer, &food->rect );
}

void generateFood( Food *food, Snakes *snakelist) {
    food->rect.x = ( rand() % COLS ) * SIZE ;
    food->rect.y = ( rand() % ROWS ) * SIZE;

    Snake *head = NULL;
    SDL_Point food_coo = {food->rect.x, food->rect.y};

    int index = 0;
    
    while ( index < playerNum ) {
        head = snakelist->snake[index];

        // Ensures food will spawn on empty spaces
        if ( SDL_PointInRect( &food_coo, &head->rect ) ) {
            generateFood( food, SnakeList );
            return;
        }
        Snake *body = head->body;
        Snake *prevBody = NULL;

        while ( body ) {
            if ( SDL_PointInRect( &food_coo, &body->rect ) ) {
                generateFood( food, SnakeList );
                return;
            }
            prevBody = body;
            body = prevBody->body;
        }
        if ( SDL_PointInRect( &food_coo, &head->tail->rect ) ) {
            generateFood( food, SnakeList );
            return;
        }
        index ++;
    }
}

void checkFoodhasEaten( Snake *snake, Food *food ) {
    if ( !food ) {
        return;
    }
    if ( SDL_HasIntersection( &snake->rect, &food->rect) == SDL_TRUE ) {
        generateFood( food , SnakeList);
        growSnake( snake, 1 );
        // growBody( snake );
        // snake->SPEED += SIZE;
    }
}

void moveFood( Food *food ) {
    // Moves food based on keyboard input
    if ( !food || !foodAlive ) {
        return;
    }

    if ( food->FoodBuffDir != food->FoodDIR ) {
        foodChunkTurn( food );
        food->FoodDIR = food->FoodBuffDir;
        food->LASTDIR = food->FoodDIR;
    }

    int pixels = (int) (food->FoodSPEED * delta_time);
    switch ( food->FoodDIR ) {
        case LEFT:
            food->rect.x -= pixels;
            break;
        case RIGHT:
            food->rect.x += pixels;
            break;
        case UP:
            food->rect.y -= pixels;
            break;
        case DOWN:
            food->rect.y += pixels;
            break;
        default:
            break;
    }
}

void foodChunkTurn( Food *food ) {
    // Ensure food will turn every chunk
    int x_coo = food->rect.x;
    int y_coo = food->rect.y; 

    // Chunks
    // Lower bound x,y
    int lower_bound_x = x_coo - ( x_coo % SIZE );
    int lower_bound_y = y_coo - ( y_coo % SIZE );

    // Upper bound x,y
    int upper_bound_x = lower_bound_x + SIZE;
    int upper_bound_y = lower_bound_y + SIZE;

    // Middle bound x,y
    int middle_bound_x = ( lower_bound_x +  upper_bound_x ) / 2;
    int middle_bound_y = ( lower_bound_y + upper_bound_y ) / 2;

    switch ( food->FoodDIR ) {
        // Left and Right turn
        case DOWN:
            // Lower bound bias
            if ( food->rect.y % SIZE != 0 ) 
            {
                // Check if bottom has passed quarter of the chunk
                food->rect.y = ( y_coo + SIZE < middle_bound_y + SIZE 
                && ( food->LASTLASTDIR != RIGHT && food->LASTLASTDIR != LEFT ) ) ?
                lower_bound_y : upper_bound_y;

                food->LASTLASTDIR = food->LASTDIR;
            }
            break;

        case UP: 
            // Upper bound bias 
            if ( food->rect.y % SIZE != 0 ) 
            {
                food->rect.y = ( y_coo > middle_bound_y 
                && ( food->LASTLASTDIR != RIGHT && food->LASTLASTDIR != LEFT ) ) ?
                upper_bound_y : lower_bound_y;

                food->LASTLASTDIR = food->LASTDIR;
            }
            break;

        // Up and Down turn
        case RIGHT:
            // Lower bound bias
            if ( food->rect.x % SIZE != 0 ) 
            {
                // Check if top right passed quarter of the chunk
                food->rect.x = ( x_coo + SIZE < middle_bound_x + SIZE 
                && ( food->LASTLASTDIR != UP && food->LASTLASTDIR != DOWN ) ) ?
                lower_bound_x : upper_bound_x;

                food->LASTLASTDIR = food->LASTDIR;
            }
            break;
            
        case LEFT:
            // Upper bound bias
            if ( food->rect.x % SIZE != 0 ) 
            {
                food->rect.x = ( x_coo > middle_bound_x 
                && ( food->LASTLASTDIR != UP && food->LASTLASTDIR != DOWN ) ) ?
                upper_bound_x : lower_bound_x;

                food->LASTLASTDIR = food->LASTDIR;
            }
            break;
    }
}

void deleteFood( Food *food ) {
    if ( !food ) {
        return; 
    }
    free(food);
}



