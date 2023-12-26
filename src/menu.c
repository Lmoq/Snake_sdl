#include <stdio.h>
#include "main.h"
#include "menu.h"
#include "utils.h"


void initTTF() {
    TTF_Init();
}

void setup_menu() {
    // Initialize ttf lib
    initTTF();

    font = NULL;
    text_color = NULL;
    texts = NULL;
    
    // Open font file
    font = TTF_OpenFont( CASCADIA , 30 );
    if ( !font ) printf("Font failed \n");

    // Setup text
    SDL_Color text_color_ = { 255, 255, 255, 255 };
    text_color = &text_color_;

    // Place texts
    addText( &texts, "Amogus", false, WIDTH, 100, 0 );
    addText( &texts, "1 Player", true, WIDTH, 300, ONEPLAYER );
    addText( &texts, "2 Player", true, WIDTH, 350, TWOPLAYER );
    addText( &texts, "3 Player", true, WIDTH, 400, THREEPLAYER );
    addText( &texts, "PLAY", true, WIDTH, 500, START );

}

void addText( text **pText, char *string, bool query, int x, int y, int option ) {
    if ( !(*pText) ) {
        // Initialize first element in linked list
        *pText = malloc (sizeof ( text ));

        if ( !(*pText) ) {
            fprintf( stderr, "Malloc failed\n" );
            running = false;
            return;
        }
        createTexture( *pText, string, query, x, y, option );
        ( *pText )->nextText = NULL;

    } else {
        // Add next element to nextText
        text *currentText = *pText;
        text *prevText = NULL;

        while ( currentText ) {
            prevText = currentText;
            currentText = prevText->nextText;
        }
        prevText->nextText = malloc( sizeof(text) );

        if (prevText->nextText == NULL) {
            fprintf( stderr, "Malloc failed\n" );
            running = false;
            return;
        }
        createTexture( prevText->nextText, string, query, x, y, option );
        prevText->nextText->nextText = NULL;
    }
}

void createTexture( text *pText, char *string, bool query, int x, int y, int option ) {
    // Fill in the texture struct of pText
    pText->surface = TTF_RenderText_Solid( font, string, *text_color );
    pText->texture = SDL_CreateTextureFromSurface( renderer, pText->surface );

    pText->option = option;

    pText->rect.w = 200;
    pText->rect.h = 50;

    if ( query ) SDL_QueryTexture( pText->texture, NULL, NULL, &pText->rect.w, &pText->rect.h );

    pText->rect.x = ( x == WIDTH ) ? ( WIDTH / 2 ) -  ( pText->rect.w / 2 ) : x;
    pText->rect.y = y;
}


void listen_menu() {
    if ( event.type == SDL_MOUSEBUTTONDOWN ) {
        SDL_Point mouseClick = { event.button.x, event.button.y };
        int option = getCommand( mouseClick, texts );

        switch( option ) {
            case ONEPLAYER:
                printf("1P selected\n");
                playerNum = 0;
                break;
            case TWOPLAYER:
                printf("2P selected\n");
                playerNum = 1;
                break;
            case THREEPLAYER:
                printf("3P selected\n");
                playerNum = 2;
                break;
            case FOURPLAYER:
                printf("4P selected\n");
                playerNum = 3;
                break;
            case START:
                menu = false;
                setup();
        }  
    }

}

int getCommand( SDL_Point point, text *pText ) {
    text *current = pText;
    text *prev = NULL;

    while ( current ) {
        if ( SDL_PointInRect( &point, &current->rect ) ) {
            return current->option;
        }
        prev = current;
        current = prev->nextText;
    }
}


void update_menu(){
    
    return;
}

void show_texts( text *pText ) {
    // Renders all textures inside pTexts
    text *current = pText;
    text *prev = NULL;

    while ( current ) {
        SDL_RenderCopy( renderer, current->texture, NULL, &current->rect );
        SDL_RenderDrawRect( renderer, &current->rect );

        prev = current;
        current = prev->nextText;
    }
}

void show_menu() {
    SDL_SetRenderDrawColor( renderer , 200, 200, 200, 255 );
    show_texts( texts );
}

void delete_texts( text * pText ) {
    text *current = pText;
    text *prev = NULL;
    int count = 0;

    while ( current ) {
        SDL_FreeSurface( current->surface );
        SDL_DestroyTexture( current->texture );

        prev = current;
        current = prev->nextText;
        free( prev );
        count ++;
    }
    printf("Freed texts addresses : %d\n", count);
}


void quitTTF() {
    delete_texts( texts );
    TTF_CloseFont( font );
    TTF_Quit();
}