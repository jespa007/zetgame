#ifndef __TEXTURE_MANAGER_H__
#define __TEXTURE_MANAGER_H__

typedef struct TextureManager TextureManager;

struct TextureManager{
	void *data;
};

// STATIC
Texture * 		TextureManager_GetDefaultTexture(void);
void			TextureManager_DeInit(void);

// MEMBERS
TextureManager *TextureManager_New(void);
void 			TextureManager_SetTextureResourcePath(TextureManager *_this,const char * path);
Texture * 		TextureManager_Get(TextureManager *_this,const char * _filename);
void			TextureManager_Delete(TextureManager *_this);


#endif
