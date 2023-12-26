#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "main.h"
#include "food.h"
#include "snake.h"
#include "snake_food.h"
#include "utils.h"

void initSnake( Snakes *pSnake_ , int p1Color) {
    // Setup pSnake_ list
    if ( pSnake_ == NULL ) {
        fprintf( stderr, "Malloc failed" );
        running = false;
        return;
    } else printf("malloc sucess");
    STARTPOS1.x = 100;
    STARTPOS1.y = 100;

    STARTPOS2.x = 1100;
    STARTPOS2.y = 600;

    STARTPOS3.x = 1100;
    STARTPOS3.y = 100;

    STARTPOS4.x = 100;
    STARTPOS4.y = 600;

    pSnake_->numSnakes = 0;

    // Add a snake to list
    Snake *head = malloc(sizeof (*head));
    pSnake_->numSnakes ++;

    if ( !head ) {
        fprintf( stderr, "Malloc failed" );
        running = false;
        return;
    }

    // Set appearance
    head->rect.w = SIZE;
    head->rect.h = SIZE;
    head->COLOR = p1Color;

    pSnake_->numSnakes ++;

    // Initialize snake lead trail
    switch ( pSnake_->numSnakes ) {
        case 1:
            head->rect.x = STARTPOS1.x;
            head->rect.y = STARTPOS1.y;

            // Set movement
            head->DIRECTION = DOWN;
            head->BUFFDIR = RIGHT;
            head->SPEED = SIZE * 25;
            break;

        case 2:
            head->rect.x = STARTPOS2.x;
            head->rect.y = STARTPOS2.y;

            // Set movement
            head->DIRECTION = UP;
            head->BUFFDIR = LEFT;
            head->SPEED = SIZE * 25;
            break;

        case 3:
            head->rect.x = STARTPOS3.x;
            head->rect.y = STARTPOS3.y;

            // Set movement
            head->DIRECTION = DOWN;
            head->BUFFDIR = LEFT;
            head->SPEED = SIZE * 25;
            break;

        case 4:
            head->rect.x = STARTPOS4.x;
            head->rect.y = STARTPOS4.y;

            // Set movement
            head->DIRECTION = UP;
            head->BUFFDIR = RIGHT;
            head->SPEED = SIZE * 25;
            break;
    }

    // Reset pointers
    head->body = NULL;
    head->bodyEnd = NULL;
    head->tail = NULL;
    head->nextHead = NULL;

    // Attach tail
    head->tail = malloc (sizeof( Snake ));

    if ( !head->tail ) {
        fprintf( stderr, "Malloc failed" );
        running = false;
        return;
    }

    // Set pointers
    head->tail->body = NULL;
    head->tail->bodyEnd = NULL;
    head->tail->nextHead = NULL;
    head->tail->tail = NULL;

    // Appearance
    head->tail->rect.w = SIZE;
    head->tail->rect.h = SIZE;

    // Set movement
    head->LASTDIR = head->BUFFDIR;
    head->snakeDead = false;

    // Add to list
    pSnake_->snake = head;

    // Set initial size
    growSnake( head, STARTSIZE );
}

void addSnake( Snakes *pSnake_ ) {
    // Add a snake to list
    if ( !pSnake_ ) return;
    
    Snake *nextHead = pSnake_->snake->nextHead;
    Snake *prev = pSnake->snake;

    while ( nextHead != NULL ) {
        prev = nextHead;
        nextHead = prev->nextHead;
    }

    Snake *head = malloc( sizeof(* head) );

    if ( !head ) {
        fprintf( stderr, "Malloc failed\n" );
        running = false;
        return;
    }

    // Set appearance
    head->rect.w = SIZE;
    head->rect.h = SIZE;

    pSnake_->numSnakes ++;
    // Initialize snake lead trail
    switch ( pSnake_->numSnakes ) {
        case 1:
            head->rect.x = STARTPOS1.x;
            head->rect.y = STARTPOS1.y;

            // Set movement
            head->DIRECTION = DOWN;
            head->BUFFDIR = RIGHT;
            head->SPEED = SIZE * 25;
            break;

        case 2:
            head->rect.x = STARTPOS2.x;
            head->rect.y = STARTPOS2.y;

            // Set movement
            head->DIRECTION = UP;
            head->BUFFDIR = LEFT;
            head->SPEED = SIZE * 25;
            head->COLOR = RED;
            break;

        case 3:
            head->rect.x = STARTPOS3.x;
            head->rect.y = STARTPOS3.y;

            // Set movement
            head->DIRECTION = DOWN;
            head->BUFFDIR = LEFT;
            head->SPEED = SIZE * 25;
            head->COLOR = ORANGE;
            break;

        case 4:
            head->rect.x = STARTPOS4.x;
            head->rect.y = STARTPOS4.y;

            // Set movement
            head->DIRECTION = UP;
            head->BUFFDIR = RIGHT;
            head->SPEED = SIZE * 25;
            break;
    }
    
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
        return;
    }

    // Set pointers
    head->tail->body = NULL;
    head->tail->bodyEnd = NULL;
    head->tail->tail = NULL;
    head->tail->rect.w = SIZE;
    head->tail->rect.h = SIZE;

    head->LASTDIR = head->BUFFDIR;
    head->snakeDead = false;

    // Add to list
    prev->nextHead = head;
    growSnake( head, STARTSIZE );
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

    // Set body spawn position
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
    
    // Update head bodyEnd for tail to track
    head->bodyEnd = body;
    
    // Connect added body to last
    prevBody->body = body;
}

void deleteSnake( Snakes *pSnake_ ) {
    // Delete snake head to tail
    if ( !pSnake_ ) {
        printf("pSnake not initalized ...\nreturning...\n");
        return;
    }
    int objects = 0;
    int snakeCount = 0;

    Snake *head = pSnake_->snake;
    Snake *prevHead;

    while ( head != NULL ) {
        Snake **body = &( head->body );
        Snake *prevBody;

        while ( *body != NULL ) {
            prevBody = *body;
            body = &(prevBody->body);

            // Release all head bodies
            free( prevBody );
            printf("Freeing snake[%d] bodies total : %d\n", snakeCount, objects);
            objects ++;
        }
        prevHead = head;
        head = prevHead->nextHead;

        // Release all snake heads and tails
        free( prevHead );
        free( prevHead->tail );
        objects += 2;
        snakeCount ++;
    }

    free( pSnake_ );
    objects ++;
    
    printf("Freed addr : %d\n", objects);
}
