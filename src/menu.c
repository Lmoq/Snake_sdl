#include <stdio.h>
#include "main.h"
#include "menu.h"
#include "utils.h"


void initTTF() {
    TTF_Init();
}

// Sets up text to display on screen
void setup_menu() {
    // Initialize ttf lib
    initTTF();

    font = NULL;
    text_color = NULL;
    texts = NULL;

    // Effects
    currentHighlighted = NULL;
    
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

    // Set option value
    pText->option = option;

    // Set rect for texture
    pText->rect.w = 200;
    pText->rect.h = 50;
    pText->rectColor = PALEWHITE;

    if ( query ) SDL_QueryTexture( pText->texture, NULL, NULL, &pText->rect.w, &pText->rect.h );

    // Center rect if x == WIDTH or out of bounds
    pText->rect.x = ( x <= WIDTH ) ? ( WIDTH / 2 ) -  ( pText->rect.w / 2 ) : x;
    pText->rect.y = y;
}

// Listen for mouse clicks on texts
void listen_menu() {
    if ( event.type == SDL_MOUSEBUTTONDOWN ) {
        SDL_Point mouseClick = { event.button.x, event.button.y };
        text *pText = getCommand( mouseClick, texts );

        if ( pText ) {
            highlightText( pText );

            switch( pText->option ) {
                case ONEPLAYER:
                    printf("1P selected\n");
                    playerNum = 1;
                    break;
                case TWOPLAYER:
                    printf("2P selected\n");
                    playerNum = 2;
                    break;
                case THREEPLAYER:
                    printf("3P selected\n");
                    playerNum = 3;
                    break;
                case FOURPLAYER:
                    printf("4P selected\n");
                    playerNum = 4;
                    break;
                case START:
                    menu = false;
                    setup();
            }
        }
    }

    else if ( event.type == SDL_MOUSEMOTION ) {
        SDL_Point mousePos = {event.motion.x, event.motion.y};
        checkMousePos( mousePos, texts );
    }
}

text *getCommand( SDL_Point point, text *pText ) {
    // Returns the text pointer if mouse clicked is inside text's rect
    text *current = pText;
    text *prev = NULL;

    while ( current ) {
        if ( SDL_PointInRect( &point, &current->rect ) ) {
            return current;
        }
        prev = current;
        current = prev->nextText;
    }
    return NULL;
}

void highlightText( text *pText ) {
    // Highlights selected text
    if ( currentHighlighted ) 
    {
        // Change color of selected text if it clicked different from previous one
        if ( currentHighlighted->option != pText->option && pText->option != 0 ) {
            pText->rectColor = ORANGE;

            // Reset previous highlighted rect color
            currentHighlighted->rectColor = PALEWHITE;
            currentHighlighted = pText;
        }
    }

    // Directly set the clicked text rect to activated COLOR is previous highlighted is NULL
    else if ( !currentHighlighted && pText->option != 0 ) {
        // Activated COLOR
        pText->rectColor = ORANGE;
        currentHighlighted = pText;
    }
}

void checkMousePos( SDL_Point mousepos, text *pText ) {
    // Highlights whem mouse hover over text
    text *current = pText;
    text *prev = NULL;

    while ( current ) {
        // Highlights hovered text with Green
        if ( SDL_PointInRect( &mousepos, &current->rect ) && current != currentHighlighted && current->option != 0 ) {
            current->rectColor = GREEN;
            return;
        }
        // Reset every text rect color to PALEWHITE
        // Every move that mouse is not in any text rect
        if ( currentHighlighted != current ) {
            current->rectColor = PALEWHITE;
        }
        prev = current;
        current = prev->nextText;
    }
}


// Render
void show_texts( text *pText ) {
    // Renders all textures inside pTexts
    text *current = pText;
    text *prev = NULL;

    while ( current ) {
        setRenderColor( current->rectColor );
        SDL_RenderCopy( renderer, current->texture, NULL, &current->rect );
        SDL_RenderDrawRect( renderer, &current->rect );

        prev = current;
        current = prev->nextText;
    }
}

void show_menu() {
    show_texts( texts );
}

void delete_texts( text * pText ) {
    // Deletes Surface and Texture inside text structs
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