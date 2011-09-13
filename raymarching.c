#include "SDL/SDL.h"
#include "math.h"

//defines are cheaper than variables
#define WIDTH   640
#define HEIGHT  480
#define BPP 4
#define DEPTH 32

#define min(x,y) x<y?x:y

//raymarching functions
#define MAX_DIST  10000
#define EPSILON 0.1


void setpixel(SDL_Surface *screen, int x, int y, Uint8 r, Uint8 g, Uint8 b){

    Uint32 *pixmem32;
    Uint32 color;  
 
    color = SDL_MapRGB( screen->format, r, g, b );
  
    pixmem32 = (Uint32*) screen->pixels  + y + x;
    *pixmem32 = color;
}

float DistanceFunc(float x, float y, float z){


	float min_t = MAX_DIST;
	//sphere at 0,0,100 with radius 100
	min_t = min( min_t, sqrt(x*x + y*y + (z-100)*(z-100))-100 ); 
	
	min_t = min( min_t, sqrt((x+200)*(x+200) + (y-50)*(y-50) + (z-100)*(z-100))-80 ); 
	//cube at 100,100,100 with side length 50
	//_t = min( min_t, min(min(fabs(x-100),fabs(y-100)),fabs(z-100))-50);
	return min_t;
}

void trace(SDL_Surface* screen, int x, int y){

	float t = 0, d = 0;
	float o1 ,o2 ,o3 ,d1,d2,d3, len;
	int* v = screen->pixels;
	//origin	
	o1 = o2 = 0;
	o3 = -1*HEIGHT;
	//direction
	d1 = -.5*WIDTH + x - o1;
	d2 = -.5*HEIGHT + y - o2;   
	d3 =  0-o3;//z
	//normalize direction
	len = d1*d1+d2*d2+d3*d3; 
	len = sqrt(len);
	d1/=len; d2/=len; d3/=len;
	while(t < MAX_DIST)
	{
		d = DistanceFunc( o1+d1*t, o2+d2*t, o3+d3*t);
		if( d < EPSILON ) break; 
		t += d;
	}
	//if(x==y)
		//printf("%d:%d = %4.2f\n",x,y,t);
	if( t >= MAX_DIST  ){// No "collision" 
		v[y * WIDTH + x]  = 255<<16;
		//setpixel(screen,x,y,255,0,0);
		//pixelRGBA(screen, x, y, 255,0,0);
	}else{ //do some shading with rayStart + rayDir * t
		//pixelRGBA(screen, x, y, 0,0,255);
		//setpixel(screen,x,y,0,0,255);	
		v[y * WIDTH + x] = 255<<16 | 255<<8 | 255 ; 	
	}
}

void DrawScreen(SDL_Surface* screen)
{ 
    int x,y;

    //SDL_FillRect(screen, 0, 0);
    //SDL_LockSurface(screen);
    
    for(y = 0; y < HEIGHT; y++ ) 
    {
        for( x = 0; x < WIDTH; x++ ) 
        {

		trace(screen,x,y);
        }
    }

    //SDL_UnlockSurface(screen);
    //SDL_UpdateRect (screen, 0, 0,0,0);
    SDL_Flip(screen); 
}


void _start()
{
    SDL_Event event;
    SDL_Surface *screen;
    screen = SDL_SetVideoMode (WIDTH, HEIGHT,  32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_HWACCEL /*|SDL_FULLSCREEN*/ );

  
    do{
	DrawScreen(screen);
	SDL_PollEvent(&event);
	
    } while ( event.type!=SDL_KEYDOWN );

    SDL_Quit();

asm ( \
         "movl $1,%eax\n" \
         "xor %ebx,%ebx\n" \
         "int $128\n" \
         );

}

