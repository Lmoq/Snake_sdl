#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "main.h"
#include "food.h"
#include "snake.h"
#include "snake_food.h"
#include "utils.h"


//  Render
void drawSnakes( Snakes *pSnake_ ) {
    if (!pSnake_) {
        // printf("No list\n");
        return;
    }

    Snake *head = pSnake_->snake;
    Snake *prevHead;

    int snakeCount = 0;

    while ( head != NULL ) {
        // Draw head
        switch ( head->COLOR ) {
            case BLUE:
                SDL_SetRenderDrawColor( renderer, 0, 0, 255, 255 );
                break;
            case RED:
                SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
                break;
            case ORANGE:
                SDL_SetRenderDrawColor( renderer, 255, 165, 0, 255 );
                break;
            case GREEN:
                SDL_SetRenderDrawColor( renderer, 0, 255, 0, 255 );
                break;
        }

        // Draw heads
        SDL_RenderDrawRect( renderer, &head->rect );
        SDL_RenderFillRect( renderer, &head->rect );

        // Draw tails
        // SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
        SDL_RenderDrawRect( renderer, &head->tail->rect );
        SDL_RenderFillRect( renderer, &head->tail->rect );

        // Draw trails
        drawTrails( head, head->COLOR);

        prevHead = head;
        head = prevHead->nextHead;
        snakeCount ++;
    } 

}

void drawTrails( Snake *head, int color ) {
    // SDL_SetRenderDrawColor( renderer, 0, 0, 255, 255 );
    Snake *body = head->body;
    Snake *prevBody;

    if (!body) {
        return;
    }

    while ( body != NULL ) {
        // Render color
        switch ( color ) {
            case BLUE:
                SDL_SetRenderDrawColor( renderer, 0, 0, 255, 255 );
                break;
            case RED:
                SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
                break;
            case ORANGE:
                SDL_SetRenderDrawColor( renderer, 255, 165, 0, 255 );
                break;
            case GREEN:
                SDL_SetRenderDrawColor( renderer, 0, 255, 0, 255 );
                break;
        }
        SDL_RenderDrawRect( renderer, &body->rect );
        // SDL_RenderFillRect( renderer, &body->rect );
        prevBody = body;
        body = prevBody->body;
    }
}