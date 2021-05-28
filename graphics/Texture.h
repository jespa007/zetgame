#ifndef __TExTURE_H__
#define __TExTURE_H__

#define INVALID_TEXTURE_HANDLE -1

typedef enum{
	TEXTURE_TYPE_IMAGE=0,
	TEXTURE_TYPE_CUBEMAP
}TextureType;



typedef struct Texture Texture;

struct Texture{
	void *texture_data;
	uint16_t width, height;
	uint8_t bytes_per_pixel;
	TextureType texture_type;
};

// static
void	  	Texture_Init(void);

void 		Texture_SetTextureResourcePath(const char *resource_path);

Texture * 	Texture_GetDefault(void);
Texture * 	Texture_New(void);
Texture * 	Texture_LoadFromFile(const char *file);
//Texture * 	Texture_NewCircle(uint16_t radius, uint32_t fill_color, uint16_t border_width, uint32_t border_color);
//Texture * 	Texture_NewRectangle(uint16_t radius, uint32_t fill_color, uint16_t border_width, uint32_t border_color);
//Texture * 	Texture_NewCross(uint16_t radius, uint32_t fill_color, uint16_t border_width, uint32_t border_color);
//Texture * 	Texture_NewArrow(uint16_t radius, uint32_t fill_color, uint16_t border_width, uint32_t border_color);
Texture * 	Texture_LoadFromMemory(uint8_t *ptr, size_t size_len);

//Texture * 	Texture_Load(const char *image);
//Texture	*	Texture_Get(const char *name);

void 		Texture_Bind(Texture *_this);
void	 	Texture_SetRepeatUV(Texture *_this, bool _repeat_uv);
int		 	Texture_GetHandle(Texture *_this);
bool 	  	Texture_Update(Texture * _this,void *_pixels, uint16_t _width, uint16_t _height, uint8_t _bytes_per_pixel);
bool	  	Texture_UpdateFromSurface(Texture *_this,SDL_Surface * srf);
void	  	Texture_Delete(Texture * _this);

void	  	Texture_DeInit(void);



#endif
