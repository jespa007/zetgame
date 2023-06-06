#ifndef __ZG_TEXTURE_MANAGER_H__
#define __ZG_TEXTURE_MANAGER_H__

typedef struct ZG_TextureManager ZG_TextureManager;

struct ZG_TextureManager{
	void *data;
};

// STATIC
ZG_Texture * 		ZG_TextureManager_GetDefaultTexture(void);
void				ZG_TextureManager_DeInit(void);

// MEMBERS
ZG_TextureManager *	ZG_TextureManager_New(void);
void 				ZG_TextureManager_SetTextureResourcePath(ZG_TextureManager *_this,const char * path);
ZG_Texture * 		ZG_TextureManager_Get(ZG_TextureManager *_this,const char * _filename);
void				ZG_TextureManager_Delete(ZG_TextureManager *_this);


#endif
