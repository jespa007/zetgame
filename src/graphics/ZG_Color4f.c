#include "zg_graphics.h"



ZG_Color4f ZG_Color4f_New4f(float r, float g, float b, float a){
	return (ZG_Color4f){.r=r,.g=g,.b=b,.a=a};
}

ZG_Color4f ZG_Color4f_New3f(float r, float g, float b){
	return (ZG_Color4f){.r=r,.g=g,.b=b,.a=1};
}


ZG_Color4f ZG_Color4f_FromRGB(uint8_t r, uint8_t g, uint8_t b){
	return (ZG_Color4f){.r=r*ZG_ONE_OVER_256,.g=g*ZG_ONE_OVER_256,.b=b*ZG_ONE_OVER_256,ZG_ALPHA_VALUE_SOLID};
}

ZG_Color4f ZG_Color4f_FromRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
	return (ZG_Color4f){.r=r*ZG_ONE_OVER_256,.g=g*ZG_ONE_OVER_256,.b=b*ZG_ONE_OVER_256,.a=a*ZG_ONE_OVER_256};
}

ZG_Color4f ZG_Color4f_FromHex(uint32_t hex){
	return (ZG_Color4f){
				 .a=((hex&0xFF000000)>>24)*ZG_ONE_OVER_256
				,.r=((hex&0x00FF0000)>>16)*ZG_ONE_OVER_256
				,.g=((hex&0x0000FF00)>>8)*ZG_ONE_OVER_256
				,.b=((hex&0x000000FF)>>0)*ZG_ONE_OVER_256

		};
}

ZG_Color4f ZG_Color4f_FromHtml(const char * _html){
	char *html=(char *)_html;
	if(_html==NULL || *_html==0) {
		ZG_Log_ErrorF("string empty or null");
		return ZG_COLOR4F_WHITE;
	}

	if(*html!='#') {
		ZG_Log_ErrorF("Invalid hex colorit should start by # (example #0e0e0e)");
		return ZG_COLOR4F_WHITE;
	}


	int hex;

	if(ZG_String_StringToInt(&hex,html+1,16)){
		return (ZG_Color4f){
				 .a=((hex&0xFF000000)>>24)*ZG_ONE_OVER_256
				,.r=((hex&0x00FF0000)>>16)*ZG_ONE_OVER_256
				,.g=((hex&0x0000FF00)>>8)*ZG_ONE_OVER_256
				,.b=((hex&0x000000FF)>>0)*ZG_ONE_OVER_256

		};
	}
	return ZG_COLOR4F_WHITE;
}


ZG_Color4f Color4f_FromString(const char * color){
	if(STRCMP(color,==,"white")){
		return (ZG_Color4f){1,1,1,1};
	}

	return (ZG_Color4f){0,0,0,0};
}
