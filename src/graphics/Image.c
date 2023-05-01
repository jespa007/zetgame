#include "zg_graphics.h"

typedef struct{
	SDL_Surface *srf;
}ImageData;

Image *Image_NewFromSurface(SDL_Surface *_srf){
	Image *img=ZG_NEW(Image);
	ImageData *data=ZG_NEW(ImageData);

	data->srf = _srf;
	img->data=data;
	return img;
}

Image * Image_NewFromFile(const char *_filename){
	BufferByte *buffer= NULL;
	Image *image = NULL;

	if(FileSystem_FileExists(_filename)){
		buffer=FileSystem_ReadFile(_filename);

		image=Image_NewFromMemory(buffer->ptr,buffer->len);
		BufferByte_Delete(buffer);

	}else{
		Log_Error("File '%s' not exist",_filename);
	}
	return image;
}

Image * 	Image_NewFromMemory(uint8_t *ptr, size_t ptr_len){
	SDL_Surface * srf=NULL;

	if((srf=SDL_LoadImageFromMemory(ptr,ptr_len,0,0))!=NULL){
		return Image_NewFromSurface(srf);
	}

	return NULL;
}

Image *Image_New(uint16_t _width, uint16_t _height){
	Image *img=ZG_NEW(Image);
	ImageData *data=ZG_NEW(ImageData);
	uint8_t bytes_per_pixel=Graphics_GetBytesPerPixel();
	data->srf = SDL_NewSurface(_width,_height,bytes_per_pixel);;

	img->data=data;

	return img;
}

uint16_t 		Image_GetHeight(Image *_this){
	ImageData *data=_this->data;

	return data->srf->h;
}

uint16_t 		Image_GetWidth(Image *_this){
	ImageData *data=_this->data;
	return data->srf->w;
}

uint8_t 	*	Image_GetPixels(Image *_this){
	ImageData *data=_this->data;
	return data->srf->pixels;
}

void Image_Delete(Image * _this){

	ImageData *data=_this->data;

	SDL_FreeSurface(data->srf);

	ZG_FREE(data);
	ZG_FREE(_this);

}
