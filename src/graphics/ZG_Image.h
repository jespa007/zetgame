#ifndef __ZG_IMAGE_H__
#define __ZG_IMAGE_H__

typedef struct ZG_Image ZG_Image;

struct ZG_Image{
	void *data;
};

#define		ZG_IMAGE_CONVERT_PROPERTY_FLIP_Y			 				(0x1 << 1)
#define		ZG_IMAGE_CONVERT_PROPERTY_POWER_OF_2_ORIGINAL_RESOLUTION	(0x1 << 2)

//-------
// STATIC

ZG_Image	* 	ZG_Image_Convert(
	ZG_Image *_src_image
	, uint32_t _dst_convert_properties
	, uint8_t _dst_bytes_per_pixel
);

ZG_Image 	*	ZG_Image_LoadImageFromFile(
	const char *_file
	, uint32_t _dst_convert_properties
	, uint8_t _dst_bytes_per_pixel

);
ZG_Image 	*	ZG_Image_LoadImageFromMemory(
	const uint8_t *_ptr
	, size_t _ptr_len
	, uint32_t _dst_convert_properties
	, uint8_t _dst_bytes_per_pixel
);

//
// Create from image formats from file or memory
//
void			ZG_Image_DestroyResources(void);

//-------
// MEMBER
// Creates a new Image with bytes per pixels as is tells ZG_Graphics_GetBytesPerPixel
ZG_Image 	*	ZG_Image_New(
	uint16_t _with
	,uint16_t _height
	,uint8_t _bytes_per_pixel
);

ZG_Image	* 	ZG_Image_NewFrom(
	uint8_t *_pixels
	, uint16_t _width
	, uint16_t _height
	, uint8_t bytes_per_pixel
);



uint16_t 		ZG_Image_GetHeight(ZG_Image *_this);
uint16_t 		ZG_Image_GetWidth(ZG_Image *_this);
uint8_t 	*	ZG_Image_GetPixels(ZG_Image *_this);
uint8_t 		ZG_Image_GetBytesPerPixel(ZG_Image *_this);
uint8_t 		ZG_Image_GetBitsPerPixel(ZG_Image *_this);

// Image management

void 			ZG_Image_SetPixel(ZG_Image *surface, int x, int y, uint32_t pixel);
ZG_Image 	* 	ZG_Image_NewImage(uint16_t _width, uint16_t _height, uint8_t bytes_per_pixel);
ZG_Image 	* 	ZG_Image_NewCircle(uint16_t radius, uint32_t fill_color, uint16_t border_width, uint32_t border_color);
ZG_Image 	* 	ZG_Image_NewCross(uint16_t dimension, uint32_t fill_color, uint16_t border_width);
ZG_Image 	* 	ZG_Image_NewRectangle(uint16_t dimension, uint32_t fill_color, uint16_t border_width, uint32_t border_color);
ZG_Image 	* 	ZG_Image_NewTriangle(uint16_t dimension, uint32_t fill_color, uint16_t border_width, uint32_t border_color);
ZG_Image 	* 	ZG_Image_NewArrow(uint16_t dimension, uint32_t fill_color, uint16_t border_width, uint32_t border_color);
void 			ZG_Image_Blit(ZG_Image * _dst_image,ZG_Rectanglei *_dst_rect,ZG_Image * _src_image,ZG_Rectanglei *_src_rect);

ZG_Image 	*	ZG_Image_Crop(ZG_Image *_src,ZG_Rectanglei _rect);

bool 		 	ZG_Image_SavePNG(ZG_Image 	* _this, const char * _filename );
bool			ZG_Image_SaveJPG(ZG_Image 	* _this, const char * _filename);
void 			ZG_Image_FlipY(ZG_Image 	* _this);

void 			ZG_Image_Delete(ZG_Image 	*_this);

#endif
