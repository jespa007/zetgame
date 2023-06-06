#include "_zg_graphics_.h"

typedef struct{
	SDL_Surface *srf;
}ZG_ImageData;

ZG_Image *ZG_Image_NewFromSurface(SDL_Surface *_srf){
	ZG_Image *img=ZG_NEW(ZG_Image);
	ZG_ImageData *data=ZG_NEW(ZG_ImageData);

	data->srf = _srf;
	img->data=data;
	return img;
}

ZG_Image * ZG_Image_NewFromFile(const char *_filename){
	ZG_BufferByte *buffer= NULL;
	ZG_Image *image = NULL;

	if(FileSystem_FileExists(_filename)){
		buffer=FileSystem_ReadFile(_filename);

		image=ZG_Image_NewFromMemory(buffer->ptr,buffer->len);
		ZG_BufferByte_Delete(buffer);

	}else{
		ZG_Log_Error("File '%s' not exist",_filename);
	}
	return image;
}

ZG_Image * 	ZG_Image_NewFromMemory(uint8_t *ptr, size_t ptr_len){
	SDL_Surface * srf=NULL;

	if((srf=SDL_LoadImageFromMemory(ptr,ptr_len,0,0))!=NULL){
		return ZG_Image_NewFromSurface(srf);
	}

	return NULL;
}

ZG_Image *ZG_Image_New(uint16_t _width, uint16_t _height){
	ZG_Image *img=ZG_NEW(ZG_Image);
	ZG_ImageData *data=ZG_NEW(ZG_ImageData);
	uint8_t bytes_per_pixel=ZG_Graphics_GetBytesPerPixel();
	data->srf = SDL_NewSurface(_width,_height,bytes_per_pixel);;

	img->data=data;

	return img;
}

uint16_t 		ZG_Image_GetHeight(ZG_Image *_this){
	ZG_ImageData *data=_this->data;

	return data->srf->h;
}

uint16_t 		ZG_Image_GetWidth(ZG_Image *_this){
	ZG_ImageData *data=_this->data;
	return data->srf->w;
}

uint8_t 	*	ZG_Image_GetPixels(ZG_Image *_this){
	ZG_ImageData *data=_this->data;
	return data->srf->pixels;
}

void ZG_Image_Delete(ZG_Image * _this){

	ZG_ImageData *data=_this->data;

	SDL_FreeSurface(data->srf);

	ZG_FREE(data);
	ZG_FREE(_this);

}
