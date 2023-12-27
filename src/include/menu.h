#ifndef MENU_H
#define MENU_H
#include <SDL2/SDL_ttf.h>

#define START 500
#define ONEPLAYER 501
#define TWOPLAYER 502
#define THREEPLAYER 503
#define FOURPLAYER 504

TTF_Font *font;
SDL_Color *text_color;

typedef struct text {
    int option;
    int rectColor;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect rect;

    struct text *nextText;
} text;
text *texts ;

text *currentHighlighted;

void initTTF();

// Options texts
void addText( text **pText, char *string, bool query, int x, int y, int option );
void createTexture( text *pText, char *string, bool query, int x, int y, int option );
void setup_menu();

// Listen
void listen_menu();

// Update
text *getCommand( SDL_Point point, text *pText );
void checkMousePos( SDL_Point mousepos, text *pText );
void highlightText( text *pText );

// Render
void show_texts( text *pText );
void show_menu();

// Clean up
void delete_texts( text *pText );
void quitTTF();

#endif