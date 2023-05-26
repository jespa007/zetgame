#ifndef __EC_TEXTURE_H__
#define __EC_TEXTURE_H__

typedef struct ECTexture ECTexture;

struct ECTexture{
	EComponentHeader header;
	Texture 		*texture;
};


void ECTexture_Setup(void *_this,ComponentId _id);
void ECTexture_SetTexture(ECTexture *_this,Texture *_texture);


#endif
