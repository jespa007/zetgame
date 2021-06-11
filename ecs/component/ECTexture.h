#ifndef __EC_TEXTURE_H__
#define __EC_TEXTURE_H__

typedef struct ECTexture ECTexture;

struct ECTexture{
	EComponentHeader;
	Texture 	*texture;
};


void ECTexture_Setup(void *_this, Entity *entity);
void ECTexture_SetTexture(ECTexture *_this,Texture *_texture);
void ECTexture_Destroy(void *_this);



#endif
