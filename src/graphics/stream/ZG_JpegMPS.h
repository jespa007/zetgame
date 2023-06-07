#ifndef __JPEG_MPS_H__
#define __JPEG_MPS_H__

typedef struct ZG_JpegMPS ZG_JpegMPS;

struct ZG_JpegMPS{
	void *data;
};

ZG_JpegMPS * 	ZG_JpegMPS_New(void);
void 		JpegMPS_Start(ZG_JpegMPS * _this, int port);
void 		JpegMPS_Stop(ZG_JpegMPS * _this);
void 		ZG_JpegMPS_Delete(ZG_JpegMPS * _this);


#endif
