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
        return;
    }

    Snake *head = pSnake_->snake;
    Snake *prevHead;

    while ( head != NULL ) {
        // Use snake color for render
        setRenderColor( head->COLOR );

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
        setRenderColor( color );
        SDL_RenderDrawRect( renderer, &body->rect );
        // SDL_RenderFillRect( renderer, &body->rect );

        prevBody = body;
        body = prevBody->body;
    }
}

void setRenderColor( int COLOR ) {
    switch ( COLOR ) {
        case BLUE: 
            setRenderColorBLUE;
            break;
        case RED:
            setRenderColorRED;
            break;
        case ORANGE:
            setRenderColorORANGE;
            break;
        case GREEN:
            setRenderColorGREEN;
            break;
        case WHITE:
            setRenderColorWHITE;
            break;
        case PALEWHITE:
            setRenderColorPALEWHITE;
            break;
        case BLACK:
            setRenderColorBLACK;
            break;
        default:
            break;
    }
}