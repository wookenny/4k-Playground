#include "SDL/SDL.h"

#include "sys/soundcard.h"
#include "fcntl.h"
#include "sys/ioctl.h"
#include "unistd.h"

//defines are cheaper than variables
#define WIDTH   800
#define HEIGHT  600


void drawWhite(SDL_Surface *screen){
	unsigned x,y;
	SDL_LockSurface(screen);
	for(y=0; y<HEIGHT; ++y)
		for(x=0; x<WIDTH; ++x)
			pixelRGBA(screen, x,y,255,255,255);
	SDL_LockSurface(screen);
	SDL_UpdateRect (screen, 0, 0, WIDTH, HEIGHT);
	return;	
}

void drawThings(SDL_Surface *screen, float MinRe, float MaxRe,float MinIm) 
{
	/*
	int i,j,r,g,b;
	for(j = 4;j<HEIGHT-4;++j){
		r = rand()%256; g = rand()%256; b = rand()%256;
		for(i =4; i<WIDTH-4;++i)
			 	pixelRGBA(screen, i, j, r,g,b);
	}
	*/
	//draw mandelbrot set
	{
		//static float MinRe = -2.0;
		//static float MaxRe = 2.0;
		//static float MinIm = -1.4;	
		float MaxIm = MinIm+(MaxRe-MinRe)*HEIGHT/WIDTH;
		float Re_factor = (MaxRe-MinRe)/(WIDTH-1);
		float Im_factor = (MaxIm-MinIm)/(HEIGHT-1);
		unsigned MaxIterations = 30;
		unsigned y,x,n;
		float c_im, c_re, Z_re, Z_re2, Z_im, Z_im2;
		int isInside;
		int r,g,b;

		SDL_LockSurface(screen);
		for(y=0; y<HEIGHT; ++y)
		{

			//r = rand() % 256; g = rand()%256; b = rand()%256;
			c_im = MaxIm - y*Im_factor;
			for(x=0; x<WIDTH; ++x)
			{
				c_re = MinRe + x*Re_factor;

				Z_re = c_re, Z_im = c_im;
				isInside = 1;
				for(n=0; n<MaxIterations; ++n)
				{
				    Z_re2 = Z_re*Z_re, Z_im2 = Z_im*Z_im;
				    if(Z_re2 + Z_im2 > 4)
				    {
				        isInside = 0;
				        break;
				    }
				    Z_im = 2*Z_re*Z_im + c_im;
				    Z_re = Z_re2 - Z_im2 + c_re;
				}
				if(isInside)pixelRGBA(screen, x,y,0,0,0);
				else pixelRGBA(screen, x,y,(8*255*n)%256,0,0);
			
				//if(!isInside) {pixelRGBA(screen, x,y,255*n/,0,0); }
				//else{ pixelRGBA(screen, x, y, 200,0,0);}
			}
		}
	}
	SDL_UnlockSurface(screen);
	SDL_UpdateRect (screen, 0, 0, WIDTH, HEIGHT);
	return;
}

void _start()
{
    SDL_Event event;
    SDL_Surface *screen;
    int audio_fd,i;
    short audio_buffer[4096];

    audio_fd = open("/dev/dsp", O_WRONLY, 0);
    i=AFMT_S16_LE;ioctl(audio_fd,SNDCTL_DSP_SETFMT,&i);
    i=1;ioctl(audio_fd,SNDCTL_DSP_CHANNELS,&i);
    i=11024;ioctl(audio_fd,SNDCTL_DSP_SPEED,&i);

    for (i=0;i<4096;i++)
    {
        audio_buffer[i]=i<<8;
    }

    screen = SDL_SetVideoMode (WIDTH, HEIGHT,  32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_HWACCEL | SDL_FULLSCREEN );

   
    float MinRe=-2., MaxRe=2., MinIm= -1.;	
    do{

	//printf("1\n");	
	
        drawThings(screen,MinRe,MaxRe,MinIm);	
	//SDL_UpdateRect (screen, 0, 0, WIDTH, HEIGHT);

	//ioctl(audio_fd,SNDCTL_DSP_SYNC);
        //write(audio_fd,audio_buffer,8192);
        
	//drawWhite(screen);
	MinRe /= 1.01;
	MaxRe /= 1.01;
	MinIm /= 1.05;
	//MinIm -= 0.001;
	SDL_PollEvent(&event);
	
    } while ( event.type!=SDL_KEYDOWN );

    SDL_Quit();

    asm ( \
         "movl $1,%eax\n" \
         "xor %ebx,%ebx\n" \
         "int $128\n" \
         );

}

