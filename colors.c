#include "SDL/SDL.h"

//defines are cheaper than variables
#define WIDTH   800
#define HEIGHT  600
#define BPP 4
#define DEPTH 32

void setpixel(SDL_Surface *screen, int x, int y, Uint8 r, Uint8 g, Uint8 b)
{
    Uint32 *pixmem32;
    Uint32 color;  
 
    color = SDL_MapRGB( screen->format, r, g, b );
  
    pixmem32 = (Uint32*) screen->pixels  + y + x;
    *pixmem32 = color;
}


void DrawScreen(SDL_Surface* screen, int h)
{ 
    int x, y, ytimesw;
  
    SDL_LockSurface(screen);
    

    for(y = 0; y < screen->h; y++ ) 
    {
        ytimesw = y*screen->pitch/BPP;
        for( x = 0; x < screen->w; x++ ) 
        {
            setpixel(screen, x, ytimesw, (x*x)/256+3*y+h, (y*y)/256+x+h, h);
        }
    }

    SDL_UnlockSurface(screen);
  
    SDL_Flip(screen); 
}


void _start()
{
    SDL_Event event;
    SDL_Surface *screen;
    int h = 0;
    screen = SDL_SetVideoMode (WIDTH, HEIGHT,  32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_HWACCEL /*|SDL_FULLSCREEN*/ );

  
    do{
	DrawScreen(screen,h++);
	SDL_PollEvent(&event);
	
    } while ( event.type!=SDL_KEYDOWN );

    SDL_Quit();

    asm ( \
         "movl $1,%eax\n" \
         "xor %ebx,%ebx\n" \
         "int $128\n" \
         );

}

