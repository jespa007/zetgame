#ifndef __ZG_IMAGE_H__
#define __ZG_IMAGE_H__

typedef struct ZG_Image ZG_Image;

struct ZG_Image{
	void *data;
};

ZG_Image * 		ZG_Image_NewFromMemory(uint8_t *ptr, size_t ptr_len);
ZG_Image *		ZG_Image_NewFromFile(const char *_file);
ZG_Image *		ZG_Image_New(uint16_t _with,uint16_t _height);

uint16_t 		ZG_Image_GetHeight(ZG_Image *_this);
uint16_t 		ZG_Image_GetWidth(ZG_Image *_this);
uint8_t 	*	ZG_Image_GetPixels(ZG_Image *_this);


void 			ZG_Image_Delete(ZG_Image *_this);

#endif
