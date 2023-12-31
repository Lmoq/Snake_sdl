#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "main.h"
#include "food.h"
#include "snake.h"
#include "snake_food.h"
#include "utils.h"

Snakes *initSnake( ) {
    // Initialize Snakes
    STARTPOS1.x = 100;
    STARTPOS1.y = 100;

    STARTPOS2.x = 1100;
    STARTPOS2.y = 600;

    STARTPOS3.x = 1100;
    STARTPOS3.y = 100;

    STARTPOS4.x = 100;
    STARTPOS4.y = 600;

    Snakes *snakelist = malloc( sizeof(* snakelist) );

    if ( !snakelist ) {
        fprintf( stderr, "Malloc failed\n" );
        running = false;
        return NULL;
    }

    snakelist->numSnakes = 0;
    snakelist->snake = malloc( sizeof(Snake *) * MAXPLAYERS );

    // Fill slots with snakes
    for (int i = 0; i < MAXPLAYERS; i ++) {
        snakelist->numSnakes ++;
        snakelist->snake[i] = addSnake( snakelist->numSnakes );
    }

    return snakelist;
}

Snake *addSnake( int snakeIndex ) {
    // Initlialize a snakelist slots
    Snake *head = malloc( sizeof(* head) );

    if ( !head ) {
        fprintf( stderr, "Malloc failed\n" );
        running = false;
        return NULL;
    }

    // Set appearance
    head->rect.w = SIZE;
    head->rect.h = SIZE;

    // Set snake start pos
    setSnakePos( head, snakeIndex );
    
    // Reset pointers
    head->body = NULL;
    head->bodyEnd = NULL;
    head->tail = NULL;
    head->nextHead = NULL;

    // Initialize tail
    head->tail = malloc (sizeof( Snake ));

    if ( !head->tail ) {
        fprintf( stderr, "Malloc failed" );
        running = false;
        return NULL;
    }

    // Set members
    head->tail->body = NULL;
    head->tail->bodyEnd = NULL;
    head->tail->tail = NULL;

    head->LASTDIR = head->BUFFDIR;
    head->snakeDead = false;
    head->deathTimeStamp = 0;
    head->snakeIndex = snakeIndex;

    head->tail->rect.w = SIZE;
    head->tail->rect.h = SIZE;

    growSnake( head, STARTSIZE );
    return head;
}

void setSnakePos( Snake *head , int snakeIndex) {
    switch ( snakeIndex ) {
        case 1:
            head->rect.x = STARTPOS1.x;
            head->rect.y = STARTPOS1.y;

            // Set movement
            head->DIRECTION = DOWN;
            head->BUFFDIR = RIGHT;
            head->SPEED = PSPEED;
            head->COLOR = BLUE;
            break;

        case 2:
            head->rect.x = STARTPOS2.x;
            head->rect.y = STARTPOS2.y;

            // Set movement
            head->DIRECTION = UP;
            head->BUFFDIR = LEFT;
            head->SPEED = PSPEED;
            head->COLOR = RED;
            break;

        case 3:
            head->rect.x = STARTPOS3.x;
            head->rect.y = STARTPOS3.y;

            // Set movement
            head->DIRECTION = DOWN;
            head->BUFFDIR = LEFT;
            head->SPEED = PSPEED;
            head->COLOR = ORANGE;
            break;

        case 4:
            head->rect.x = STARTPOS4.x;
            head->rect.y = STARTPOS4.y;

            // Set movement
            head->DIRECTION = UP;
            head->BUFFDIR = RIGHT;
            head->SPEED = PSPEED;
            break;
    }
}

void growSnake( Snake *head, int size ) {
    // Repeatedly call growBody to grow snake
    for ( int i = 0; i < size; i++ ) {
        growBody( head );
    }
}

void growBody( Snake *head ) {
    // Extend head body
    Snake *body = head->body;
    Snake *prevBody = head;

    while ( body != NULL ) {
        prevBody = body;
        body = prevBody->body;
    }
    
    // Create new bodyEnd
    body = malloc( sizeof(*body) );
    if ( !body ) {
        fprintf(stderr, "Malloc failed");
        running = false;
    }

    // Set body spawn position and adjust tail
    switch ( prevBody->LASTDIR ) {
        case RIGHT:
            body->rect.x = prevBody->rect.x - SIZE;
            body->rect.y = prevBody->rect.y;

            head->tail->rect.x = body->rect.x;
            head->tail->rect.y = body->rect.y;
            break;
        case LEFT:
            body->rect.x = prevBody->rect.x + SIZE;
            body->rect.y = prevBody->rect.y;

            head->tail->rect.x = body->rect.x;
            head->tail->rect.y = body->rect.y;
            break;
        case DOWN:
            body->rect.x = prevBody->rect.x;
            body->rect.y = prevBody->rect.y - SIZE;

            head->tail->rect.x = body->rect.x;
            head->tail->rect.y = body->rect.y;
            break;
        case UP:
            body->rect.x = prevBody->rect.x;
            body->rect.y = prevBody->rect.y + SIZE;

            head->tail->rect.x = body->rect.x;
            head->tail->rect.y = body->rect.y;
            break;
        default:
            break;
    }
    body->rect.w = SIZE;
    body->rect.h = SIZE;
    body->body = NULL;
    body->tail = NULL;
    body->LASTDIR = prevBody->LASTDIR;
    
    // Update bodyEnd for tail to follow
    head->bodyEnd = body;

    // Connect added body to last
    prevBody->body = body;
}

void setupKeys() {
    // Player 1 keys
    p1Key.LEFTKEY = SDL_SCANCODE_A;
    p1Key.RIGHTKEY = SDL_SCANCODE_D;
    p1Key.UPKEY = SDL_SCANCODE_W;
    p1Key.DOWNKEY = SDL_SCANCODE_S;

    // Player 2 keys
    p2Key.LEFTKEY = SDL_SCANCODE_LEFT;
    p2Key.RIGHTKEY = SDL_SCANCODE_RIGHT;
    p2Key.UPKEY = SDL_SCANCODE_UP;
    p2Key.DOWNKEY = SDL_SCANCODE_DOWN;

    // Player 3 keys
    p3Key.LEFTKEY = SDL_SCANCODE_J;
    p3Key.RIGHTKEY = SDL_SCANCODE_L;
    p3Key.UPKEY = SDL_SCANCODE_I;
    p3Key.DOWNKEY = SDL_SCANCODE_K;
}

void deleteHead( Snake *head ) {
    // Deletes head, bodies and tail
    if (!head) {
        printf("Trying to delete null head\n");
        return;
    }
    Snake *body = head->body;
    Snake *prevBody = NULL;

    while (body) {
        prevBody = body;
        body = prevBody->body;

        free(prevBody);
    }
    free( head->tail );
    free( head );
}

void deleteSnakes( Snakes *snakelist ) {
    // Deletes all snakes
    if ( !snakelist ) {
        printf("pSnake not initalized ...\nreturning...\n");
        return;
    }
    int index = 0;
    Snake *head = NULL;
    
    while ( index < playerNum ) {
        head = snakelist->snake[index];
        // Deletes head and trails
        deleteHead( head );
        index ++;
    }
    free(snakelist);
}
