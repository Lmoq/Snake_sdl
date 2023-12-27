#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "main.h"
#include "food.h"
#include "snake.h"
#include "snake_food.h"
#include "utils.h"


void moveSnakes( Snakes *pSnake_, Food *food ) {
    if ( !pSnake_ ) {
        return;
    }
    int sCount = 0;

    Snake *head = pSnake_->snake;
    Snake *prevHead;

    while ( head != NULL ) {
        if ( head->snakeDead && head && head->body ) {
            prevHead = head;
            head = prevHead->nextHead;
            continue;
        }

        // Follow trails
        moveBody( head );

        // Lock snake movement
        if ( head->BUFFDIR != head->DIRECTION ) {
            // Ensure snake head turns are locked by chunk
            chunkTurn( head );

            // Recall moveBody to sync when head makes a chunk turn
            // before updating lastdir
            moveBody( head );

            head->DIRECTION = head->BUFFDIR;
            head->LASTDIR = head->DIRECTION;
        }

        // Continously move snake head / lead body
        switch ( head->DIRECTION ) {
            case LEFT:
                head->rect.x -= (int) (head->SPEED * delta_time);
                break;
            case RIGHT:
                head->rect.x += (int) (head->SPEED * delta_time);
                break;
            case UP:
                head->rect.y -= (int) (head->SPEED * delta_time);
                break;
            case DOWN:
                head->rect.y += (int) (head->SPEED * delta_time);
                break;
            default:
                break;
        }
        moveTail( head );
        // Check collisions
        checkSnakeCol( head , head);
        checkOtherCol( head );
        checkFoodhasEaten( head, food );

        prevHead = head;
        head = prevHead->nextHead;
        sCount ++;
    }

    // printf("Scount : %d\n", sCount);
}

void moveBody( Snake *head ) {
    // Makes bodytrails follow snake head when head completely covers the next chunk
    Snake *body = head->body;
    Snake *prevBody = head;
    int count = 0;

    switch ( prevBody->LASTDIR ) {
        case LEFT:
            if ( prevBody->rect.x + SIZE <= body->rect.x ) {
                // Ensures body trails can only move chunk by chunk
                body->rect.x -= SIZE;

                // Sync body trails and tail
                moveBodyTrails( body, head );
                updateTail( head );
                head->LASTLASTDIR = head->LASTDIR;

                // Update lastdir after all trails have been updated
                body->LASTDIR = prevBody->LASTDIR;
            }
            break;

        case RIGHT:
            if ( prevBody->rect.x >= body->rect.x + SIZE ) {
                body->rect.x += SIZE;
      
                moveBodyTrails( body, head );
                updateTail( head );
                head->LASTLASTDIR = head->LASTDIR;
                body->LASTDIR = prevBody->LASTDIR;
            }
            break;

        case UP:
            if ( prevBody->rect.y + SIZE <= body->rect.y ) {
                body->rect.y -= SIZE;
      
                moveBodyTrails( body, head );
                updateTail( head );
                head->LASTLASTDIR = head->LASTDIR;
                body->LASTDIR = prevBody->LASTDIR;
            }
            break;

        case DOWN:
            if ( prevBody->rect.y >= body->rect.y + SIZE ) {
                body->rect.y += SIZE;
      
                moveBodyTrails( body, head );
                updateTail( head );
                head->LASTLASTDIR = head->LASTDIR;
                body->LASTDIR = prevBody->LASTDIR;
            }
            break;

        default:
            break;
    }
}

void moveBodyTrails( Snake *body, Snake *head ) {
    // Move all trailing bodies
    Snake *body_body = body->body;
    Snake *prevBody = body;

    if ( body_body != NULL ) {
        switch ( prevBody->LASTDIR ) {
            case LEFT:
                body_body->rect.x -= SIZE;
                moveBodyTrails( body_body, head );
                // Update lastdir for tail to follow bodyEnd.lastDir before bodyEnd makes a turn
                body_body->LASTDIR = prevBody->LASTDIR;
                break;

            case RIGHT:
                body_body->rect.x += SIZE;
                moveBodyTrails( body_body, head );

                body_body->LASTDIR = prevBody->LASTDIR;
                break;

            case UP:
                body_body->rect.y -= SIZE;
                moveBodyTrails( body_body, head );

                body_body->LASTDIR = prevBody->LASTDIR;
                break;

            case DOWN:
                body_body->rect.y += SIZE;
                moveBodyTrails( body_body, head );

                body_body->LASTDIR = prevBody->LASTDIR;
                break;

            default:
                break;
        } 
    }
}

void moveTail( Snake *head ){
    if (!head->tail) {
        fprintf( stderr, "No tail" );
    }
    // Only move tail when bodyEnd has already moved
    // so tail wont overrun bodyEnd and get revealed
    if (head->tail->rect.x != head->bodyEnd->rect.x || 
        head->tail->rect.y != head->bodyEnd->rect.y) 
    {
        switch ( head->bodyEnd->LASTDIR ) {
            case LEFT:
                head->tail->rect.x -= (int) (head->SPEED * delta_time);
                break;
            case RIGHT:
                head->tail->rect.x += (int) (head->SPEED * delta_time);
                break;
            case UP:
                head->tail->rect.y -= (int) (head->SPEED * delta_time);
                break;
            case DOWN:
                head->tail->rect.y += (int) (head->SPEED * delta_time);
                break;
            default:
                break;
        }
    }
}

void updateTail( Snake *head ) {
    // Sync tail position at the back of bodyEnd
    switch ( head->bodyEnd->LASTDIR ) {
        case LEFT:
            head->tail->rect.x = head->bodyEnd->rect.x + SIZE;
            head->tail->rect.y = head->bodyEnd->rect.y;
            break;
        case RIGHT:
            head->tail->rect.x = head->bodyEnd->rect.x - SIZE;
            head->tail->rect.y = head->bodyEnd->rect.y;
            break;
        case UP:
            head->tail->rect.x = head->bodyEnd->rect.x;
            head->tail->rect.y = head->bodyEnd->rect.y + SIZE;
            break;
        case DOWN:
            head->tail->rect.x = head->bodyEnd->rect.x;
            head->tail->rect.y = head->bodyEnd->rect.y - SIZE;
            break;
    }
    
}
void chunkTurn( Snake *head ) {
    // Ensure snake head will turn every chunk
    int x_coo = head->rect.x;
    int y_coo = head->rect.y; 

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

    switch ( head->DIRECTION ) {
        // Left and Right turn
        case DOWN:
            // Lower bound bias
            if ( head->rect.y % SIZE != 0 ) 
            {
                // Check if bottom has passed quarter of the chunk
                head->rect.y = ( y_coo + SIZE < middle_bound_y + SIZE 
                && ( head->LASTLASTDIR != RIGHT && head->LASTLASTDIR != LEFT ) ) ?
                lower_bound_y : upper_bound_y;
            }
            break;

        case UP: 
            // Upper bound bias 
            if ( head->rect.y % SIZE != 0 ) 
            {
                head->rect.y = ( y_coo > middle_bound_y 
                && ( head->LASTLASTDIR != RIGHT && head->LASTLASTDIR != LEFT ) ) ?
                upper_bound_y : lower_bound_y;
            }
            break;

        // Up and Down turn
        case RIGHT:
            // Lower bound bias
            if ( head->rect.x % SIZE != 0 )
            {
                // Check if top right passed quarter of the chunk
                head->rect.x = ( x_coo + SIZE < middle_bound_x + SIZE 
                && ( head->LASTLASTDIR != UP && head->LASTLASTDIR != DOWN ) ) ?
                lower_bound_x : upper_bound_x;
            }
            break;

        case LEFT:
            // Upper bound bias
            if ( head->rect.x % SIZE != 0 ) 
            {
                head->rect.x = ( x_coo > middle_bound_x 
                && ( head->LASTLASTDIR != UP && head->LASTLASTDIR != DOWN ) ) ?
                upper_bound_x : lower_bound_x;
            }
            break;
    }
}

void checkSnakeCol( Snake *head, Snake *targetHead ) {
    Snake *body = head->body->body;
    Snake *prev;

    targetHead->pointHead.x = targetHead->rect.x;
    targetHead->pointHead.y = targetHead->rect.y;

    if ( targetHead->COLOR != head->COLOR && SDL_PointInRect( &targetHead->pointHead, &head->rect ) ) {
        targetHead->snakeDead = true;
    }

    while ( body != NULL ) {
        if ( SDL_PointInRect( &head->pointHead, &body->rect ) == SDL_TRUE ) {
            targetHead->snakeDead = true;
        }
        prev = body;
        body = prev->body;
    }

    if ( SDL_PointInRect( &targetHead->pointHead, &head->tail->rect ) == SDL_TRUE ) {
        targetHead->snakeDead = true;
    }
}

void checkOtherCol( Snake *head ) {
    Snake *otherHead = pSnake->snake;
    Snake *prev = NULL;

    while ( otherHead ) {
        // Verify otherHead is not self if colors are different
        if ( head->COLOR != otherHead->COLOR ) {
            checkSnakeCol( otherHead, head );
        }
        prev = otherHead;
        otherHead = prev->nextHead;
    }
}