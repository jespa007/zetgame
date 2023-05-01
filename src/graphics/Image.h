#ifndef __IMAGE_H__
#define __IMAGE_H__

typedef struct Image Image;

struct Image{
	void *data;
};

Image * 		Image_NewFromMemory(uint8_t *ptr, size_t ptr_len);
Image *			Image_NewFromFile(const char *_file);
Image *			Image_New(uint16_t _with,uint16_t _height);

uint16_t 		Image_GetHeight(Image *_this);
uint16_t 		Image_GetWidth(Image *_this);
uint8_t 	*	Image_GetPixels(Image *_this);


void 			Image_Delete(Image *_this);

#endif
