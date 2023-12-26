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
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect rect;

    struct text *nextText;
} text;
text *texts ;

void initTTF();
void addText( text **pText, char *string, bool query, int x, int y, int option );
void createTexture( text *pText, char *string, bool query, int x, int y, int option );
void setup_menu();

void listen_menu();
void update_menu();

void show_texts( text *pText );
void show_menu();
int getCommand( SDL_Point point, text *pText );

void delete_texts( text *pText );
void quitTTF();

#endif