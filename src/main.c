#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "food.h"
#include "main.h"
#include "menu.h"
#include "snake.h"
#include "snake_food.h"
#include "utils.h"


int main(int argc, char *argv[]) {
    SDL_Init( SDL_INIT_EVERYTHING );

    running = true;
    menu = true;
    gameWindow = NULL;
    renderer = NULL;
    pSnake = NULL;

    playerNum = 0;

    // Initialize Window
    gameWindow = SDL_CreateWindow( 
        "Sample",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH,HEIGHT,
        0
    );
    if ( !gameWindow ) {
        fprintf(stderr, "Window failed to initialize");
        return 1;
    }

    // Initialize Renderer
    renderer = SDL_CreateRenderer( gameWindow, -1, 0 );
    if ( !renderer ) {
        fprintf( stderr, "Renderer failed to initialize" );
        return 1; 
    } 

    setup();

    // Game loop
    while ( running ) {
        listen();
        update();
        render();
    }
    terminate();
    printf("Exited Sucessfully");
    return 0;
}

void setup() {
    if ( menu ) {
        setup_menu();
        return;
    }

    // Initilize snake 
    pSnake = malloc( sizeof( *pSnake ) );
    initSnake( pSnake , BLUE);

    // Add players
    for ( int i = 0; i < playerNum; i ++ ) {
        addSnake( pSnake );
    }

    // Add food
    pFood = malloc( sizeof( *pFood ));
    initFood( pFood );
}

void update() {
    // Frame limiter 
    last_frame_time = SDL_GetTicks64();
    time_to_wait = FRAME_TIME - ( SDL_GetTicks64() - last_frame_time );

    // Sync update with fps
    if ( time_to_wait > 0 && time_to_wait <= FRAME_TIME ) {
        SDL_Delay( time_to_wait );
    }

    // Calculate delta time
    delta_time = (float) ( SDL_GetTicks64() - last_frame_time ) / 1000.0;

    // Show menu
    if ( menu ) {
        return;
    }
    
    // Main game update
    moveSnakes( pSnake, pFood );
    
    // Update food
    // moveFood( pFood );
}

void render() {
    // Clear screen
    SDL_SetRenderDrawColor( renderer, 0,0,0,255 );
    SDL_RenderClear( renderer );

    if ( menu ) {
        show_menu();
        // Display buffer
        SDL_RenderPresent( renderer );
        return;
    }

    // Buffer objects to draw
    drawSnakes( pSnake );
    drawFood( pFood );

    // Display buffer
    SDL_RenderPresent( renderer );
}

void listen() {
    // Event listener
    SDL_PollEvent( &event );

    if ( event.type == SDL_QUIT ) {
        running = false;
    }
    else if ( event.type == SDL_KEYDOWN ) {
        keycode = event.key.keysym.sym;
        switch ( keycode ) {
            // Change direction
            // Player 1
            case SDLK_a:
                pSnake->snake->BUFFDIR = ( pSnake->snake->DIRECTION != RIGHT ) ? LEFT : pSnake->snake->BUFFDIR;
                break;
            case SDLK_d:
                pSnake->snake->BUFFDIR = ( pSnake->snake->DIRECTION != LEFT ) ? RIGHT : pSnake->snake->BUFFDIR;
                break;
            case SDLK_w:
                pSnake->snake->BUFFDIR = ( pSnake->snake->DIRECTION != DOWN ) ? UP : pSnake->snake->BUFFDIR;
                break;
            case SDLK_s:
                pSnake->snake->BUFFDIR = ( pSnake->snake->DIRECTION != UP ) ? DOWN : pSnake->snake->BUFFDIR;
                break;
        }   
            
        // Player 2
        if ( playerNum >= 1 && pSnake->snake->nextHead ) {
            switch ( keycode ) {
                case SDLK_LEFT:
                    pSnake->snake->nextHead->BUFFDIR = ( pSnake->snake->nextHead->DIRECTION != RIGHT ) ? LEFT : pSnake->snake->nextHead->BUFFDIR;
                    break;
                case SDLK_RIGHT:
                    pSnake->snake->nextHead->BUFFDIR = ( pSnake->snake->nextHead->DIRECTION != LEFT ) ? RIGHT : pSnake->snake->nextHead->BUFFDIR;
                    break;
                case SDLK_UP:
                    pSnake->snake->nextHead->BUFFDIR = ( pSnake->snake->nextHead->DIRECTION != DOWN ) ? UP : pSnake->snake->nextHead->BUFFDIR;
                    break;
                case SDLK_DOWN:
                    pSnake->snake->nextHead->BUFFDIR = ( pSnake->snake->nextHead->DIRECTION != UP ) ? DOWN : pSnake->snake->nextHead->BUFFDIR;
                    break;
            }
        }
        // Player 3
        if ( playerNum >= 2 && pSnake->snake->nextHead->nextHead ) {
            switch ( keycode ) {
                // Food controls
                case SDLK_j:
                    pSnake->snake->nextHead->nextHead->BUFFDIR = ( pSnake->snake->nextHead->nextHead->DIRECTION != RIGHT ) ? LEFT : pSnake->snake->nextHead->nextHead->BUFFDIR;
                    break;
                case SDLK_l:
                    pSnake->snake->nextHead->nextHead->BUFFDIR = ( pSnake->snake->nextHead->nextHead->DIRECTION != LEFT ) ? RIGHT : pSnake->snake->nextHead->nextHead->BUFFDIR;
                    break;
                case SDLK_i:
                    pSnake->snake->nextHead->nextHead->BUFFDIR = ( pSnake->snake->nextHead->nextHead->DIRECTION != DOWN ) ? UP : pSnake->snake->nextHead->nextHead->BUFFDIR;
                    break;
                case SDLK_k:
                    pSnake->snake->nextHead->nextHead->BUFFDIR = ( pSnake->snake->nextHead->nextHead->DIRECTION != UP ) ? DOWN : pSnake->snake->nextHead->nextHead->BUFFDIR;
                    break;

                default:
                    break;
            }
        }
        //  Food
        // if ( pFood ) {
        //     switch ( keycode ) {
        //         // Food controls
        //         case SDLK_j:
        //             pFood->FoodBuffDir = LEFT ;
        //             break;
        //         case SDLK_l:
        //             pFood->FoodBuffDir = RIGHT;
        //             break;
        //         case SDLK_i:
        //             pFood->FoodBuffDir = UP;
        //             break;
        //         case SDLK_k:
        //             pFood->FoodBuffDir = DOWN ;
        //             break;

        //         default:
        //             break;
        //     }
    }
    if ( menu ) {
        listen_menu();
    }  
}

void terminate() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(gameWindow);
    SDL_Quit();

    deleteSnake( pSnake );
    deleteFood( pFood );
    quitTTF();
}
