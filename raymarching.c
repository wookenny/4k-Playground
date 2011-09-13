#include "SDL/SDL.h"
#include "math.h"

//defines are cheaper than variables
#define WIDTH   640
#define HEIGHT  480
#define BPP 4
#define DEPTH 32

#define min(x,y) x<y?x:y
#define max(x,y) x<y?y:x
#define length(x,y,z) sqrt(x*x+y*y+z*z)

//raymarching functions
#define MAX_DIST  10000
#define EPSILON 0.1

#define LIGHT_X 640
#define LIGHT_Y -480
#define LIGHT_Z -480


#define FPS_INTERVAL 1.0 //seconds.


// you would initalise these in a main method, really.

int spheres[] = {0,0,100,100,
		-200,50,100,80};

float DistanceFunc(float x, float y, float z){
	int i;
	float min_t = MAX_DIST;
	//sphere at 0,0,100 with radius 100
	min_t = min( min_t, sqrt( (x-spheres[0])*(x-spheres[0]) + (y-spheres[1])*(y-spheres[1]) + (z-spheres[2])*(z-spheres[2]) )-spheres[3] ); 
	min_t = min( min_t, sqrt( (x-spheres[4])*(x-spheres[4]) + (y-spheres[5])*(y-spheres[5]) + (z-spheres[6])*(z-spheres[6]) )-spheres[7] ); 

	//sinus plane with cutoff
	if(x*x + z*z < 500*500 )
		min_t = min( min_t, (200+10*sin(x*0.05)*sin(z*0.05))-y );
	return min_t;
}



int shade(float l_x, float l_y, float l_z, float n_x, float n_y, float n_z){
	//NORMALIZE BEFORE CALL
	float cos_L_N = l_x*n_x + l_y*n_y + l_z*n_z;
	cos_L_N = max(0,cos_L_N);
	return ( (int)(255*cos_L_N) <<16  | (int)(255*cos_L_N) <<8 | (int)(255*cos_L_N) ); 
	//return normal*lightvector
}


#define getNormal(x,y,z,n_x,n_y,n_z) {float len; *n_x = DistanceFunc(x+EPSILON,y,z)-DistanceFunc(x-EPSILON,y,z);	*n_y = DistanceFunc(x,y+EPSILON,z)-DistanceFunc(x,y-EPSILON,z);	   	*n_z = DistanceFunc(x,y,z+EPSILON)-DistanceFunc(x,y,z-EPSILON);	len = length(*n_x,*n_y,*n_z);  	*n_x/=len; *n_y/=len; *n_z/=len; }
/*
void getNormal( float x, float y, float z, float *n_x, float *n_y, float *n_z )
{
	 
	float len;
	*n_x = DistanceFunc(x+EPSILON,y,z)-DistanceFunc(x-EPSILON,y,z);
	*n_y = DistanceFunc(x,y+EPSILON,z)-DistanceFunc(x,y-EPSILON,z);	
   	*n_z = DistanceFunc(x,y,z+EPSILON)-DistanceFunc(x,y,z-EPSILON);	
	
	len = length(*n_x,*n_y,*n_z);   
	*n_x/=len; *n_y/=len; *n_z/=len;  
	return;
}
*/

void trace(SDL_Surface* screen, int x, int y){

	float t = 0, d = 0;
	float o1 ,o2 ,o3 ,d1,d2,d3, len;
	float p_x,p_y,p_z,n_x, n_y, n_z, l_x, l_y, l_z;
	int* v = screen->pixels;
	//origin	
	o1 = o2 = 0;
	o3 = -1*HEIGHT;
	//direction
	d1 = -.5*WIDTH + x - o1;
	d2 = -.5*HEIGHT + y - o2;   
	d3 =  0-o3;//z
	//normalize direction
	len = length(d1,d2,d3); 
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
		v[y * WIDTH + x]  = 12<<16 | 12<<8 | 80;

	}else{ //do some shading with rayStart + rayDir * t

		//calculate normal and shade it
		p_x = o1+d1*t; p_y = o2+d2*t; p_z = o3+d3*t;
		getNormal( p_x, p_y, p_z, &n_x, &n_y, &n_z );
		l_x = LIGHT_X - p_x; 
		l_y = LIGHT_Y - p_y;
		l_z = LIGHT_Z - p_z;
		len = length(l_x, l_y, l_z);
		l_x/=len; l_y/=len; l_z/=len;
		v[y * WIDTH + x] = shade(l_x, l_y, l_z,  n_x, n_y, n_z) ; 	
	}
}

void DrawScreen(SDL_Surface* screen)
{ 
	int x,y;
	for(y = 0; y < HEIGHT; y++ ) 
		for( x = 0; x < WIDTH; x++ ) 
			trace(screen,x,y);
	SDL_Flip(screen); 
	
	spheres[3] += 1; 
	spheres[7] -= 1;
}


void _start()
{
    SDL_Event event;
    SDL_Surface *screen;
    int lastTime = SDL_GetTicks();
    screen = SDL_SetVideoMode (WIDTH, HEIGHT,  32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_HWACCEL /*|SDL_FULLSCREEN*/ );

  
    do{
	DrawScreen(screen);
	SDL_PollEvent(&event);
	//while(SDL_GetTicks() - lastTime < 40 )  SDL_Delay(5); //milliseconds	
	

    } while ( event.type!=SDL_KEYDOWN );

    SDL_Quit();

	asm ( \
		 "movl $1,%eax\n" \
		 "xor %ebx,%ebx\n" \
		 "int $128\n" \
		 );

}

