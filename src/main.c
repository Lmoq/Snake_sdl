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
    foodAlive = false;
    keyCodes = NULL;
    gameWindow = NULL;
    renderer = NULL;
    SnakeList = NULL;

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

    setupKeys();
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
    // Initilize snakes
    SnakeList = initSnake();

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
    moveSnakes( SnakeList, pFood );
    
    // Update food movement
    moveFood( pFood );
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
    drawGrid( renderer );
    drawSnakes( SnakeList );
    drawFood( pFood );

    // Display buffer
    SDL_RenderPresent( renderer );
}

void checkKeys( Snake* head, const Uint8 *keycodes, PlayerKey playerkey ) {
    // Modify head.BUFFDIR based on key press
    if ( keyCodes[ playerkey.LEFTKEY ] ) {
        head->BUFFDIR = ( head->DIRECTION != RIGHT ) ? LEFT : head->BUFFDIR;
    }
    else if ( keyCodes[ playerkey.RIGHTKEY ] ) {
        head->BUFFDIR = ( head->DIRECTION != LEFT ) ? RIGHT : head->BUFFDIR;
    }
    else if ( keyCodes[ playerkey.UPKEY ] ) {
        head->BUFFDIR = ( head->DIRECTION != DOWN ) ? UP : head->BUFFDIR;
    }
    else if ( keyCodes[ playerkey.DOWNKEY ] ) {
        head->BUFFDIR = ( head->DIRECTION != UP ) ? DOWN : head->BUFFDIR;
    }
}

void listen() {
    // Event listener
    SDL_PollEvent( &event );

    if ( event.type == SDL_QUIT ) {
        running = false;
    }
    else if ( event.type == SDL_KEYDOWN ) {
        keyCodes = SDL_GetKeyboardState(NULL);
        // Player 1
        if ( SnakeList->snake[0] != NULL ) {
            checkKeys( SnakeList->snake[0], keyCodes, p1Key );
        }
        // Player 2
        if ( SnakeList->snake[1] != NULL ) {
            checkKeys( SnakeList->snake[1], keyCodes, p2Key );
        }
        // Player 3
        if ( SnakeList->snake[2] != NULL ) {
            checkKeys( SnakeList->snake[2], keyCodes, p3Key );
        }
        
        SDL_ResetKeyboard();

        
    }
    if ( menu ) {
        listen_menu();
    }  
}


void terminate() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(gameWindow);
    SDL_Quit();

    deleteSnakes( SnakeList );
    deleteFood( pFood );
    quitTTF();
}
