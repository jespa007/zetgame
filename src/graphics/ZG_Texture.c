#include "ZG_Texture_GL.c"

//--------------
// MEMBER PUBLIC
ZG_Texture * ZG_Texture_New(void *_pixels,uint16_t _width, uint16_t _height, uint8_t _bytes_per_pixel){
	ZG_Texture *text=ZG_NEW(ZG_Texture);
	memset(text,0,sizeof(ZG_Texture));

	switch(ZG_Graphics_GetGraphicsApi()){
	default:
		break;
	case ZG_GRAPHICS_API_GL:
		ZG_Texture_GL_New(text,_pixels,_width, _height, _bytes_per_pixel);
		break;
	}

	return text;
}

ZG_Texture * ZG_Texture_NewFromSurface(SDL_Surface *_image){
	return ZG_Texture_New(_image->pixels,_image->w, _image->h, _image->format->BytesPerPixel);
}

ZG_Texture * 	ZG_Texture_NewCircle(uint16_t radius, uint32_t fill_color, uint16_t border_width, uint32_t border_color){
	SDL_Surface *srf=SDL_NewCircle(radius, fill_color, border_width, border_color);
	ZG_Texture *texture=ZG_Texture_NewFromSurface(srf);
	SDL_FreeSurface(srf);
	return texture;
}

void ZG_Texture_Bind(ZG_Texture * _this){

	if(_this == NULL) return;

	switch(ZG_Graphics_GetGraphicsApi()){
	default:
		break;
	case ZG_GRAPHICS_API_GL:
		ZG_Texture_GL_Bind(_this);
		break;
	}
}

ZG_Texture * ZG_Texture_NewFromFile(const char *_filename){
	ZG_BufferByte *buffer= NULL;
	ZG_Texture *texture = NULL;

	if(ZG_FileSystem_FileExists(_filename)){
		buffer=ZG_FileSystem_ReadFile(_filename);

		texture=ZG_Texture_NewFromMemory(buffer->ptr,buffer->len);
		ZG_BufferByte_Delete(buffer);

	}else{
		ZG_Log_Error("File '%s' not exist",_filename);
	}

	return texture;

}

ZG_Texture * 	ZG_Texture_NewFromMemory(uint8_t *ptr, size_t ptr_len){
	ZG_Texture * text=NULL;
	SDL_Surface * srf=NULL;

	if((srf=SDL_LoadImageFromMemory(ptr,ptr_len,0,0))!=NULL){

		text=ZG_Texture_NewFromSurface(srf);
		SDL_FreeSurface(srf);
	}

	return text;
}

int		 	ZG_Texture_GetHandle(ZG_Texture *_this){

	if(_this == NULL) return false;

	switch(ZG_Graphics_GetGraphicsApi()){
	default:

		break;
	case ZG_GRAPHICS_API_GL:
		return ZG_Texture_GL_GetHandle(_this);
		break;
	}

	return ZG_INVALID_TEXTURE_HANDLE;
}

void 	ZG_Texture_SetRepeatUV(ZG_Texture *_this, bool repeat_uv){

	switch(ZG_Graphics_GetGraphicsApi()){
	default:
		break;
	case ZG_GRAPHICS_API_GL:
		ZG_Texture_GL_SetRepeatUV(_this,repeat_uv);
		break;
	}

}

void		ZG_Texture_SetFilter(ZG_Texture *_this, ZG_TextureFilter _filter){
	switch(ZG_Graphics_GetGraphicsApi()){
	default:
		break;
	case ZG_GRAPHICS_API_GL:
		ZG_Texture_GL_SetFilter(_this,_filter);
		break;
	}
}

bool 	  ZG_Texture_Update(ZG_Texture * _this,uint16_t _x, uint16_t _y,uint16_t _width, uint16_t _height, GLvoid *_pixels, uint8_t _bytes_per_pixel){

	if(_this == NULL) return false;

	switch(ZG_Graphics_GetGraphicsApi()){
	default:

		break;
	case ZG_GRAPHICS_API_GL:
		return ZG_Texture_GL_Update(_this,_x,_y, _width, _height,_pixels, _bytes_per_pixel);
		break;
	}

	return false;
}


bool ZG_Texture_UpdateFromSurface(ZG_Texture *_this, uint16_t _x, uint16_t _y,SDL_Surface *srf){

	if(_this == NULL) return false;

	if(srf ==NULL) {
		ZG_Log_WarningF("Surface null");
		return false;
	}
	return ZG_Texture_Update(_this,_x,_y,srf->w,srf->h,srf->pixels,srf->format->BytesPerPixel);
}

void ZG_Texture_Delete(ZG_Texture *_this){

	if(_this==NULL) return;


	switch(ZG_Graphics_GetGraphicsApi()){
	default:

		break;
	case ZG_GRAPHICS_API_GL:
		ZG_Texture_GL_Delete(_this);
		break;
	}

	ZG_FREE(_this);
}

