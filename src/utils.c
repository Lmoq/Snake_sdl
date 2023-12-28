#include <SDL2/SDL.h>
#include "main.h"
#include "utils.h"

void drawGrid( SDL_Renderer * renderer ) {
    SDL_SetRenderDrawColor( renderer, 50, 50, 50, 100);
    int x_coo;
    int y_coo;
    // Draw grid lines
    for (int i = 0; i < COLS; i ++) {   
        for (int j = 0; j < ROWS; j ++) {
            x_coo = i * SIZE;
            y_coo = j * SIZE;
            // Draw vertical lines & horizontal lines
            SDL_RenderDrawLine( renderer, x_coo, y_coo, x_coo, HEIGHT);
            SDL_RenderDrawLine( renderer, x_coo, y_coo, WIDTH, y_coo);
        }
    }
} 
