#ifndef __COLOR4F_H__
#define __COLOR4F_H__

#define 	ONE_OVER_256 			0.00390625f

#define 	ALPHA_VALUE_TRANSPARENT	0
#define 	ALPHA_VALUE_SOLID		1

#define 	COLOR4F_BLACK 			(Color4f){0.00,0.00,0.00,1.00}
#define 	COLOR4F_WHITE 			(Color4f){1.00,1.00,1.00,1.00}
#define 	COLOR4F_RED 			(Color4f){1.00,0.00,0.00,1.00}
#define 	COLOR4F_GRAY 			(Color4f){0.50,0.50,0.50,1.00}
#define 	COLOR4F_SILVER 			(Color4f){0.75,0.75,0.75,1.00}


typedef struct Color4f Color4f;

struct Color4f{
	float r,g,b,a;
};


Color4f Color4f_New(float r, float g, float b, float a);
Color4f Color4f_FromRGB(uint8_t r, uint8_t g, uint8_t b);
Color4f Color4f_FromRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
Color4f Color4f_FromHex(uint32_t hex);
Color4f Color4f_FromHtml(const char * color);

#endif
