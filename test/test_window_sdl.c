// SDL2 Hello, World!
// This should display a white screen for 2 seconds
// compile with: clang++ main.cpp -o hello_sdl2 -lSDL2
// run with: ./hello_sdl2
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdbool.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

void init()
{
  glClearColor(0,0,0,1);
  glEnable(GL_DEPTH_TEST);
}


void draw(SDL_Window *_window)
{

	  // Your rendering code goes here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-0.6f, -0.6f, 0.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.6f, -0.6f, 0.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.6f, 0.0f);
	glEnd();
	
	SDL_GL_SwapWindow(_window);
  
}

int main(int argc, char *argv[]) {
  SDL_Window* window = NULL;
  SDL_GLContext maincontext; /* Our opengl context handle */
  SDL_Surface* screenSurface = NULL;
  SDL_Event event;
  
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
    return 1;
  }
  window = SDL_CreateWindow(
			    "hello_sdl2",
			    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			    SCREEN_WIDTH, SCREEN_HEIGHT,
			    SDL_WINDOW_OPENGL |SDL_WINDOW_SHOWN
			    );
  if (window == NULL) {
    fprintf(stderr, "could not create window: %s\n", SDL_GetError());
    return 1;
  }
  
  /* Create our opengl context and attach it to our window */
  maincontext = SDL_GL_CreateContext(window);
  
  init();
  
    int running=true;
	
    while(running)
  {
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                running=false;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        running=false;
                        break;
                }
                break;

        }
    }
	
	draw(window);
	
	
  }
  
   /* Delete our opengl context, destroy our window, and shutdown SDL */
  SDL_GL_DeleteContext(maincontext);
  
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
