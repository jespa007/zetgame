#ifndef __IMAGE_MANAGER_H__
#define __IMAGE_MANAGER_H__

typedef struct ZG_ImageManager ZG_ImageManager;

struct ZG_ImageManager{
	void *data;
};

// STATIC
Image * 		ZG_ImageManager_GetDefaultImage(void);
void			ZG_ImageManager_DeInit(void);

// MEMBERS
ZG_ImageManager *	ZG_ImageManager_New(void);
void 			ZG_ImageManager_SetImageResourcePath(ZG_ImageManager *_this,const char * path);
Image * 		ZG_ImageManager_Get(ZG_ImageManager *_this,const char * _filename);
void			ZG_ImageManager_Delete(ZG_ImageManager *_this);


#endif
