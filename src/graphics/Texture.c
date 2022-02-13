#include "Texture_GL.c"

//--------------
// MEMBER PUBLIC
Texture * Texture_New(void *_pixels,uint16_t _width, uint16_t _height, uint8_t _bytes_per_pixel){
	Texture *text=ZG_NEW(Texture);
	memset(text,0,sizeof(Texture));

	switch(Graphics_GetGraphicsApi()){
	default:
		break;
	case GRAPHICS_API_GL:
		Texture_GL_New(text,_pixels,_width, _height, _bytes_per_pixel);
		break;
	}

	return text;
}

Texture * Texture_NewFromSurface(SDL_Surface *_image){
	return Texture_New(_image->pixels,_image->w, _image->h, _image->format->BytesPerPixel);
}

Texture * 	Texture_NewCircle(uint16_t radius, uint32_t fill_color, uint16_t border_width, uint32_t border_color){
	SDL_Surface *srf=SDL_NewCircle(radius, fill_color, border_width, border_color);
	Texture *texture=Texture_NewFromSurface(srf);
	SDL_FreeSurface(srf);
	return texture;
}

void Texture_Bind(Texture * _this){

	if(_this == NULL) return;

	switch(Graphics_GetGraphicsApi()){
	default:
		break;
	case GRAPHICS_API_GL:
		Texture_GL_Bind(_this);
		break;
	}
}

Texture * Texture_NewFromFile(const char *_filename){
	BufferByte *buffer= NULL;
	Texture *texture = NULL;

	if(FileSystem_FileExists(_filename)){
		buffer=FileSystem_ReadFile(_filename);

		texture=Texture_NewFromMemory(buffer->ptr,buffer->len);
		BufferByte_Delete(buffer);

	}else{
		Log_Error("File '%s' not exist",_filename);
	}

	return texture;

}

Texture * 	Texture_NewFromMemory(uint8_t *ptr, size_t ptr_len){
	Texture * text=NULL;
	SDL_Surface * srf=NULL;

	if((srf=SDL_LoadImageFromMemory(ptr,ptr_len,0,0))!=NULL){

		text=Texture_NewFromSurface(srf);
		SDL_FreeSurface(srf);
	}

	return text;
}

int		 	Texture_GetHandle(Texture *_this){

	if(_this == NULL) return false;

	switch(Graphics_GetGraphicsApi()){
	default:

		break;
	case GRAPHICS_API_GL:
		return Texture_GL_GetHandle(_this);
		break;
	}

	return INVALID_TEXTURE_HANDLE;
}

void 	Texture_SetRepeatUV(Texture *_this, bool repeat_uv){

	switch(Graphics_GetGraphicsApi()){
	default:
		break;
	case GRAPHICS_API_GL:
		Texture_GL_SetRepeatUV(_this,repeat_uv);
		break;
	}

}

void		Texture_SetFilter(Texture *_this, TextureFilter _filter){
	switch(Graphics_GetGraphicsApi()){
	default:
		break;
	case GRAPHICS_API_GL:
		Texture_GL_SetFilter(_this,_filter);
		break;
	}
}

bool 	  Texture_Update(Texture * _this,uint16_t _x, uint16_t _y,uint16_t _width, uint16_t _height, GLvoid *_pixels, uint8_t _bytes_per_pixel){

	if(_this == NULL) return false;

	switch(Graphics_GetGraphicsApi()){
	default:

		break;
	case GRAPHICS_API_GL:
		return Texture_GL_Update(_this,_x,_y, _width, _height,_pixels, _bytes_per_pixel);
		break;
	}

	return false;
}


bool Texture_UpdateFromSurface(Texture *_this, uint16_t _x, uint16_t _y,SDL_Surface *srf){

	if(_this == NULL) return false;

	if(srf ==NULL) {
		Log_WarningF("Surface null");
		return false;
	}
	return Texture_Update(_this,_x,_y,srf->w,srf->h,srf->pixels,srf->format->BytesPerPixel);
}

void Texture_Delete(Texture *_this){

	if(_this==NULL) return;


	switch(Graphics_GetGraphicsApi()){
	default:

		break;
	case GRAPHICS_API_GL:
		Texture_GL_Delete(_this);
		break;
	}

	FREE(_this);
}

