#ifndef __IMAGE_MANAGER_H__
#define __IMAGE_MANAGER_H__

typedef struct ImageManager ImageManager;

struct ImageManager{
	void *data;
};

// STATIC
Image * 		ImageManager_GetDefaultImage(void);
void			ImageManager_DeInit(void);

// MEMBERS
ImageManager *	ImageManager_New(void);
void 			ImageManager_SetImageResourcePath(ImageManager *_this,const char * path);
Image * 		ImageManager_Get(ImageManager *_this,const char * _filename);
void			ImageManager_Delete(ImageManager *_this);


#endif
