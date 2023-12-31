#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "main.h"
#include "food.h"
#include "snake.h"
#include "snake_food.h"
#include "utils.h"


void moveSnakes( Snakes *snakelist, Food *food ) {
    if ( !snakelist ) {
        return;
    }

    Snake *head = NULL;
    int index = 0;
    
    while ( index < playerNum ) {
        // Skip dead snakes
        head = snakelist->snake[index];

        if ( head->snakeDead && head && head->body ) {
            respawnSnake( &head );
            index ++;
            continue;
        }

        // Follow trails
        moveBody( head );
        // Transition if snake get past the display limit
        wrapAroundMap( head );

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

        int pixels = (int) (head->SPEED * delta_time);
        // Continously move snake head / lead body and tail
        switch ( head->DIRECTION ) {
            case LEFT:
                head->rect.x -= pixels;
                break;
            case RIGHT:
                head->rect.x += pixels;
                break;
            case UP:
                head->rect.y -= pixels;
                break;
            case DOWN:
                head->rect.y += pixels;
                break;

            default:
                break;
        }
        moveTail( head );

        // Check collisions
        checkSnakeCol( head , head);
        checkOtherCol( head , snakelist);
        checkFoodhasEaten( head, food );

        index ++;
    }
}

void moveBody( Snake *head ) {
    // Makes bodytrails follow snake head when head completely covers the next chunk
    Snake *body = head->body;
    Snake *prevBody = head;

    switch ( prevBody->LASTDIR ) {
        case LEFT:
            // Makes sure trails will follow when head wraps around map
            if ( body->rect.x + SIZE <= 0 ) {
                body->rect.x = WIDTH;
            }
            if ( prevBody->rect.x + SIZE <= body->rect.x ) {
                // Ensures body trails can only move chunk by chunk
                body->rect.x -= SIZE;

                // Sync body trails and tail to head
                // And updates head LASTLASTDIR only after passing a chunk after turns
                updateTrailsHeadLastdir( body, head, prevBody );
            } 
            break;

        case RIGHT:
            if ( body->rect.x >= WIDTH ) {
                body->rect.x = -SIZE;
            }
            if ( prevBody->rect.x >= body->rect.x + SIZE) {
                body->rect.x += SIZE;
                updateTrailsHeadLastdir( body, head, prevBody );
            }
            break;

        case UP:
            if ( body->rect.y + SIZE <= 0 ) {
                body->rect.y = HEIGHT;
            }
            if ( prevBody->rect.y + SIZE <= body->rect.y ) {
                body->rect.y -= SIZE;
                updateTrailsHeadLastdir( body, head, prevBody );
            }
            break;

        case DOWN:
            if ( body->rect.y >= HEIGHT ) {
                body->rect.y = -SIZE;
            }
            if ( prevBody->rect.y >= body->rect.y + SIZE ) {
                body->rect.y += SIZE;
                updateTrailsHeadLastdir( body, head, prevBody );
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
                // Wrap around map
                if ( body_body->rect.x + SIZE <= 0 ) {
                    body_body->rect.x = WIDTH;
                }
                body_body->rect.x -= SIZE;
                // Makes sures succeeding trails will follow before updating body lastdir
                // So they will only turn at a specific chunk
                followTrails( body_body, head, prevBody );
                break;

            case RIGHT:
                if ( body_body->rect.x >= WIDTH ) {
                    body_body->rect.x = -SIZE;
                }
                body_body->rect.x += SIZE;
                followTrails( body_body, head, prevBody );
                break;

            case UP:
                if ( body_body->rect.y + SIZE <= 0 ) {
                    body_body->rect.y = HEIGHT;
                }
                body_body->rect.y -= SIZE;
                followTrails( body_body, head, prevBody );
                break;

            case DOWN:
                if ( body_body->rect.y >= HEIGHT ) {
                    body_body->rect.y = -SIZE;
                }
                body_body->rect.y += SIZE;
                followTrails( body_body, head, prevBody );
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
        int pixels = (int) (head->SPEED * delta_time);
        switch ( head->bodyEnd->LASTDIR ) {
            case LEFT:
                head->tail->rect.x -= pixels;
                break;
            case RIGHT:
                head->tail->rect.x += pixels;
                break;
            case UP:
                head->tail->rect.y -= pixels;
                break;
            case DOWN:
                head->tail->rect.y += pixels;
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
                // This will choose opposite if lastlastdir is parallel to current direction
                // To ensure turns will not make head run over the body
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

    switch ( targetHead->DIRECTION ) {
        // Set the point to check to be the center front of the snake head
        case LEFT:
            targetHead->pointHead.x = targetHead->rect.x;
            targetHead->pointHead.y = targetHead->rect.y + (SIZE / 2);
            break;

        case RIGHT:
            targetHead->pointHead.x = targetHead->rect.x + SIZE;
            targetHead->pointHead.y = targetHead->rect.y + (SIZE / 2);
            break;

        case UP:
            targetHead->pointHead.x = targetHead->rect.x + (SIZE / 2);
            targetHead->pointHead.y = targetHead->rect.y;
            break;

        case DOWN:
            targetHead->pointHead.x = targetHead->rect.x + (SIZE / 2);
            targetHead->pointHead.y = targetHead->rect.y + SIZE;
            break;
    }

    // Check if head touched another snake head
    if ( targetHead->COLOR != head->COLOR && SDL_PointInRect( &targetHead->pointHead, &head->rect ) == SDL_TRUE ) {
        targetHead->snakeDead = true;
    }

    // Check if head touched a its own or others body
    while ( body != NULL ) {
        if ( SDL_PointInRect( &targetHead->pointHead, &body->rect ) ) {
            targetHead->snakeDead = true;
        }
        prev = body;
        body = prev->body;
    }

    // Check if target head touch its own or other tail
    if ( SDL_PointInRect( &targetHead->pointHead, &head->tail->rect ) ) {
        targetHead->snakeDead = true;
    }
}

void checkOtherCol( Snake *head , Snakes *snakelist) {
    // Check collition with other snake head
    Snake *otherHead = NULL;
    int index = 0;

    while ( index < playerNum ) {
        otherHead = snakelist->snake[index];
        // They are different snake if they have different COLOR
        if ( head->COLOR != otherHead->COLOR ) {
            checkSnakeCol( otherHead, head );
        }
        index ++;
    }
}


void respawnSnake( Snake **head ) {
    if ( !(*head)->deathTimeStamp ) {
       (*head)->deathTimeStamp = SDL_GetTicks64();
    } else {
        int timePassed = SDL_GetTicks64() - (*head)->deathTimeStamp;
        if ( timePassed > 1000 ) {
            printf("deleeing snake\n");
            (*head)->snakeDead = false;

            // Reset *head
            Snake *newHead = malloc (sizeof( *newHead ));
            if ( !newHead ) {
                fprintf( stderr, "Malloc failed\n" );
                running = false;
                return;
            }
            // Recreate snake with snakeIndex as identity
            newHead = addSnake( (*head)->snakeIndex );
            
            Snake *deadSnake = *head;
            deleteHead( deadSnake );
    
            *head = newHead;
            printf("newhead.color : %d\n", (*head)->COLOR);
        }
    }
}

void wrapAroundMap( Snake *head ) { 
    // Simulates a wrap around effect
    switch ( head->DIRECTION ) {
        case LEFT:
            if ( head->rect.x + SIZE <= 0 ) {
                head->rect.x = WIDTH;
            }
            break;
        case RIGHT:
            if ( head->rect.x >= WIDTH ) {
                head->rect.x = -SIZE;
            }
            break;
        case UP:
            if ( head->rect.y + SIZE <= 0 ) {
                head->rect.y = HEIGHT;
            }
            break;
        case DOWN:
            if ( head->rect.y >= HEIGHT) {
                head->rect.y = -SIZE;
            }
            break;
    }
}