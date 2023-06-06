#ifndef __JPEG_MPS_H__
#define __JPEG_MPS_H__

typedef struct JpegMPS JpegMPS;

struct JpegMPS{
	void *data;
};

JpegMPS * 	JpegMPS_New(void);
void 		JpegMPS_Start(JpegMPS * _this, int port);
void 		JpegMPS_Stop(JpegMPS * _this);
void 		JpegMPS_Delete(JpegMPS * _this);


#endif
