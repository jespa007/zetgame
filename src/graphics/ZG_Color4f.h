#ifndef __ZG_COLOR4F_H__
#define __ZG_COLOR4F_H__

#define 	ZG_ONE_OVER_256 			0.00390625f

#define 	ZG_ALPHA_VALUE_TRANSPARENT	0
#define 	ZG_ALPHA_VALUE_SOLID		1

#define 	ZG_COLOR4F_BLACK 			(ZG_Color4f){0.00,0.00,0.00,1.00}
#define 	ZG_COLOR4F_WHITE 			(ZG_Color4f){1.00,1.00,1.00,1.00}
#define 	ZG_COLOR4F_RED 				(ZG_Color4f){1.00,0.00,0.00,1.00}
#define 	ZG_COLOR4F_GRAY 			(ZG_Color4f){0.50,0.50,0.50,1.00}
#define 	ZG_COLOR4F_SILVER 			(ZG_Color4f){0.75,0.75,0.75,1.00}


typedef struct ZG_Color4f ZG_Color4f;

struct ZG_Color4f{
	float r,g,b,a;
};


ZG_Color4f ZG_Color4f_New4f(float r, float g, float b, float a);
ZG_Color4f ZG_Color4f_New3f(float r, float g, float b);
ZG_Color4f ZG_Color4f_FromRGB(uint8_t r, uint8_t g, uint8_t b);
ZG_Color4f ZG_Color4f_FromRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
ZG_Color4f ZG_Color4f_FromHex(uint32_t hex);
ZG_Color4f ZG_Color4f_FromHtml(const char * color);

#endif
