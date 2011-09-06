#include "GL/gl.h"
#include "SDL/SDL.h"


void _start()
{

  SDL_Event event;

  SDL_SetVideoMode(640,480,0,SDL_OPENGL|SDL_FULLSCREEN);
  SDL_ShowCursor(SDL_DISABLE);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1.33,1.33,-1,1,1.5,100);
  glMatrixMode(GL_MODELVIEW);
  glEnable(GL_DEPTH_TEST);
  glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

  glLoadIdentity();
  glBegin(GL_TRIANGLES);
  glVertex3i(1,1,-10);
  glVertex3i(1,-1,-10);
  glVertex3i(-1,1,-10);
  glEnd();
  SDL_GL_SwapBuffers();

  do
  {
    SDL_PollEvent(&event);
  } while (event.type!=SDL_KEYDOWN);
  SDL_Quit();

  asm ( \
  "movl $1,%eax\n" \
  "xor %ebx,%ebx\n" \
  "int $128\n" \
  );
}


/*
int main()
{
  SDL_Event event;

  SDL_SetVideoMode(640,480,0,SDL_OPENGL|SDL_FULLSCREEN);
  SDL_ShowCursor(SDL_DISABLE);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1.33,1.33,-1,1,1.5,100);
  glMatrixMode(GL_MODELVIEW);
  glEnable(GL_DEPTH_TEST);
  glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

  glLoadIdentity();
  glBegin(GL_TRIANGLES);
  glVertex3i(1,1,-10);
  glVertex3i(1,-1,-10);
  glVertex3i(-1,1,-10);
  glEnd();
  SDL_GL_SwapBuffers();

  do
  {
    SDL_PollEvent(&event);
  } while (event.type!=SDL_KEYDOWN);
  SDL_Quit();
}
*/

