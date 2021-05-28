#include "zg_graphics.h"



Color4f Color4f_New(float r, float g, float b, float a){
	return (Color4f){.r=r,.g=g,.b=b,.a=a};
}

Color4f Color4f_FromRGB(uint8_t r, uint8_t g, uint8_t b){
	return (Color4f){.r=r*ONE_OVER_256,.g=g*ONE_OVER_256,.b=b*ONE_OVER_256,ALPHA_VALUE_SOLID};
}

Color4f Color4f_FromRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
	return (Color4f){.r=r*ONE_OVER_256,.g=g*ONE_OVER_256,.b=b*ONE_OVER_256,.a=a*ONE_OVER_256};
}

Color4f Color4f_FromHex(uint32_t hex){
	return (Color4f){
				 .a=((hex&0xFF000000)>>24)*ONE_OVER_256
				,.r=((hex&0x00FF0000)>>16)*ONE_OVER_256
				,.g=((hex&0x0000FF00)>>8)*ONE_OVER_256
				,.b=((hex&0x000000FF)>>0)*ONE_OVER_256

		};
}

Color4f Color4f_FromHexStr(const char * _hex_str){
	char *hex_str=(char *)_hex_str;
	if(hex_str==NULL || *hex_str==0) {
		Log_Error("string empty or null");
		return COLOR4F_WHITE;
	}

	if(*hex_str!='#') {
		Log_Error("Invalid hex colorit should start by # (example #0e0e0e)");
		return COLOR4F_WHITE;
	}


	int hex;

	if(StrUtils_StrToInt(&hex,hex_str+1,16)){
		return (Color4f){
				 .a=((hex&0xFF000000)>>24)*ONE_OVER_256
				,.r=((hex&0x00FF0000)>>16)*ONE_OVER_256
				,.g=((hex&0x0000FF00)>>8)*ONE_OVER_256
				,.b=((hex&0x000000FF)>>0)*ONE_OVER_256

		};
	}
	return COLOR4F_WHITE;
}


Color4f Color4f_FromString(const char * color){
	if(STRCMP(color,==,"white")){
		return (Color4f){1,1,1,1};
	}

	return (Color4f){0,0,0,0};
}
