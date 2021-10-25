#ifndef __SDL_MEM_H__
#define __SDL_MEM_H__

#ifdef __MEMMANAGER__

#define  	SDL_CreateRGBSurface(flags,  width,  height,  depth,  Rmask,  Gmask,  Bmask,  Amask)\
															__sdlmem__SDL_CreateRGBSurface(__FILE__,__LINE__,flags,  width,  height,  depth,  Rmask,  Gmask,  Bmask,  Amask)
#define		SDL_CreateRGBSurfaceFrom(pixels,  width,  height,  depth,  pitch,  Rmask,  Gmask,  Bmask,  Amask)\
                											__sdlmem__SDL_CreateRGBSurfaceFrom(__FILE__,__LINE__,  pixels,  width,  height,  depth,  pitch,  Rmask,  Gmask,  Bmask,  Amask)
#define		SDL_ConvertSurface(src,  fmt,  flags)       	__sdlmem__SDL_ConvertSurface(__FILE__, __LINE__,  src,  fmt,  flags)
#define		SDL_FreeSurface(sdl_surface)       				__sdlmem__SDL_FreeSurface(__FILE__, __LINE__,  sdl_surface)


//SDL_CreateRGBSurface
void  MEMSDL_Init(void);
void  MEMSDL_DeInit(void);
void  MEMSDL_PrintStatus(void);

SDL_Surface	*	__sdlmem__SDL_CreateRGBSurface(const  char  *file, int  line,  uint32_t  flags,  int  width,  int  height,  int  depth,  uint32_t  Rmask,  uint32_t  Gmask,  uint32_t  Bmask,  uint32_t  Amask);
SDL_Surface	*	__sdlmem__SDL_CreateRGBSurfaceFrom(const  char  *file, int  line, void  *pixels,  int  width,  int  height,  int  depth,  int  pitch,  uint32_t  Rmask,  uint32_t  Gmask,  uint32_t  Bmask,  uint32_t  Amask);
SDL_Surface	*	__sdlmem__SDL_ConvertSurface(const  char  *file, int  line, SDL_Surface  *src,  SDL_PixelFormat  *fmt,  uint32_t  flags);
void			__sdlmem__SDL_FreeSurface(const  char  *file, int  line, void  *surface);



#endif

#endif
