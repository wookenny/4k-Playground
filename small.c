#include "SDL/SDL.h"

#include "sys/soundcard.h"
#include "fcntl.h"
#include "sys/ioctl.h"
#include "unistd.h"

void drawThings(SDL_Surface *screen)
{
	/*
	int i,j,r,g,b;
	for(j = 4;j<480-4;++j){
		r = rand()%256; g = rand()%256; b = rand()%256;
		for(i =4; i<640-4;++i)
				pixelRGBA(screen, i, j, r,g,b);
	}
	*/
	//draw mandelbrot set
	{
		float MinRe = -2.0;
		float MaxRe = 1.0;
		float MinIm = -1.2;
		float MaxIm = MinIm+(MaxRe-MinRe)*480/640;
		 float Re_factor = (MaxRe-MinRe)/(640-1);
		float Im_factor = (MaxIm-MinIm)/(480-1);
		unsigned MaxIterations = 30;
		unsigned y,x,n;
		float c_im, c_re, Z_re, Z_re2, Z_im, Z_im2;
		int isInside;
		int r,g,b;

		for(y=0; y<480; ++y)
		{

			r = rand() % 256; g = rand()%256; b = rand()%256;
			c_im = MaxIm - y*Im_factor;
			for(x=0; x<640; ++x)
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
				if(!isInside) {pixelRGBA(screen, x,y,0,0,90); }
				else{ pixelRGBA(screen, x, y, r/2,g,b);}
			}
		}
	}

	SDL_UpdateRect (screen, 0, 0, 640, 480);

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

    screen = SDL_SetVideoMode (640, 480,  32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_HWACCEL);

    do
   {
		drawThings(screen);
		//ioctl(audio_fd,SNDCTL_DSP_SYNC);
        //write(audio_fd,audio_buffer,8192);
        SDL_PollEvent(&event);

    } while (event.type!=SDL_KEYDOWN);

    SDL_Quit();

    asm ( \
         "movl $1,%eax\n" \
         "xor %ebx,%ebx\n" \
         "int $128\n" \
         );

}

