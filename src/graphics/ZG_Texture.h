#ifndef __ZG_TEXTURE_H__
#define __ZG_TEXTURE_H__

#define ZG_INVALID_TEXTURE_HANDLE -1

typedef enum{
	ZG_TEXTURE_TYPE_IMAGE=0,
	ZG_TEXTURE_TYPE_CUBEMAP
}ZG_TextureType;


typedef enum{
	ZG_TEXTURE_FILTER_LINEAR=0,
	ZG_TEXTURE_FILTER_NEAREST
}ZG_TextureFilter;



typedef struct ZG_Texture ZG_Texture;

struct ZG_Texture{
	void *texture_data;
	uint16_t width, height;
	uint8_t bytes_per_pixel;
	ZG_TextureType texture_type;
};

// public
void 		ZG_Texture_SetTextureResourcePath(const char *resource_path);


ZG_Texture * 	ZG_Texture_New(void *_pixels,uint16_t _width, uint16_t _height, uint8_t _bytes_per_pixel);
ZG_Texture *	ZG_Texture_NewFromSurface(SDL_Surface *_img);
ZG_Texture * 	ZG_Texture_NewFromFile(const char *file);
ZG_Texture * 	ZG_Texture_NewFromMemory(uint8_t *ptr, size_t ptr_len);
ZG_Texture * 	ZG_Texture_NewCircle(uint16_t radius, uint32_t fill_color, uint16_t border_width, uint32_t border_color);
ZG_Texture * 	ZG_Texture_NewRectangle(uint16_t radius, uint32_t fill_color, uint16_t border_width, uint32_t border_color);
ZG_Texture * 	ZG_Texture_NewCross(uint16_t radius, uint32_t fill_color, uint16_t border_width, uint32_t border_color);
ZG_Texture * 	ZG_Texture_NewArrow(uint16_t radius, uint32_t fill_color, uint16_t border_width, uint32_t border_color);


void		ZG_Texture_SetFilter(ZG_Texture *_this, ZG_TextureFilter _filter);
void 		ZG_Texture_Bind(ZG_Texture *_this);
void	 	ZG_Texture_SetRepeatUV(ZG_Texture *_this, bool _repeat_uv);
void	 	ZG_Texture_SetRepeatUV(ZG_Texture *_this, bool _repeat_uv);
int		 	ZG_Texture_GetHandle(ZG_Texture *_this);
bool 	  	ZG_Texture_Update(ZG_Texture * _this, uint16_t _x, uint16_t _y, uint16_t _width, uint16_t _height,void *_pixels, uint8_t _bytes_per_pixel);
bool	  	ZG_Texture_UpdateFromSurface(ZG_Texture *_this, uint16_t _x, uint16_t _y,SDL_Surface * _srf);
void	  	ZG_Texture_Delete(ZG_Texture * _this);





#endif
