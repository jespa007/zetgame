#ifndef __SDL_EXTRA_H__
#define __SDL_EXTRA_H__

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	#define RMASK32 0xff000000
	#define GMASK32 0x00ff0000
	#define BMASK32 0x0000ff00
	#define AMASK32 0x000000ff

	#define RMASK16 0xf100
	#define GMASK16 0x03e0
	#define BMASK16 0x001f
	#define AMASK16 0x0000
#else // little indian
	#define RMASK32 0x000000ff
	#define GMASK32 0x0000ff00
	#define BMASK32 0x00ff0000
	#define AMASK32 0xff000000

	#define RMASK16 0xf100
	#define GMASK16 0x03e0
	#define BMASK16 0x001f
	#define AMASK16 0x0000
#endif

#define		SDL_LOAD_IMAGE_8BIT_ALPHA 						(0x1 << 1)
#define		SDL_LOAD_IMAGE_FLIP_Y			 				(0x1 << 2)
#define		SDL_LOAD_IMAGE_POWER_OF_2_ORIGINAL_RESOLUTION	(0x1 << 3)

// BYTES PER PIXEL
typedef enum{
		COLOR_ALPHA=1,
		COLOR_RGB16,
		COLOR_RGB24,
		COLOR_RGB32
}Color;

//String [] kown_extensions = {".png",".gif",".jpg"};




// Image management
SDL_Surface * 	SDL_GetDefaultImage(void);
void			SDL_DestroyResources(void);

void 			SDL_SetPixel(SDL_Surface *surface, int x, int y, uint32_t pixel);
SDL_Surface * 	SDL_NewSurface(uint16_t w, uint16_t h, uint8_t bytes_per_pixel);
SDL_Surface * 	SDL_NewCircle(uint16_t radius, uint32_t fill_color, uint16_t border_width, uint32_t border_color);
SDL_Surface * 	SDL_NewCross(uint16_t dimension, uint32_t fill_color, uint16_t border_width);
SDL_Surface * 	SDL_NewRectangle(uint16_t dimension, uint32_t fill_color, uint16_t border_width, uint32_t border_color);
SDL_Surface * 	SDL_NewTriangle(uint16_t dimension, uint32_t fill_color, uint16_t border_width, uint32_t border_color);
SDL_Surface * 	SDL_NewArrow(uint16_t dimension, uint32_t fill_color, uint16_t border_width, uint32_t border_color);

SDL_Surface	* 	SDL_CreateSurfaceFrom(uint16_t w, uint16_t h, uint8_t bytes_per_pixel,uint8_t *pixels);
SDL_Surface	* 	SDL_ConvertSurfaceExt(SDL_Surface *surface_to_convert, uint32_t properties_format, uint8_t convert_to_bytes_per_pixel);
SDL_Surface	* 	SDL_LoadImageFromFile(const char * filename	, uint32_t properties, uint8_t convert_to_bytes_per_pixel);
SDL_Surface	*	SDL_LoadImageFromMemory(const uint8_t * image_src, size_t length, uint32_t properties_format, uint8_t convert_to_bytes_per_pixel);

SDL_Surface *	SDL_Crop(SDL_Surface *_src,SDL_Rect _rect);

bool 		 	SDL_SavePNG(const char * filename , SDL_Surface * srf);
bool			SDL_SaveJPG(const char * filename , SDL_Surface * srf);
void 			SDL_FlipY(SDL_Surface *surface);
uint32_t 		SDL_Color2Html(SDL_Color c);
SDL_Color 		SDL_Html2Color(uint32_t html);


#endif
