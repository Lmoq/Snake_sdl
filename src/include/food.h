#ifndef FOOD_H
#define FOOD_H
#include "main.h"
#include <SDL2/SDL.h>

typedef struct Food {
    int LASTDIR;
    int LASTLASTDIR;
    
    int FoodDIR;
    int FoodBuffDir;
    int FoodSPEED;

    SDL_Rect rect;
} Food ;
Food *pFood;


#endif