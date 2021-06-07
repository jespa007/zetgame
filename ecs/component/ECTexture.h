#ifndef __EC_TEXTURE_H__
#define __EC_TEXTURE_H__

typedef struct ECTexture ECTexture;

struct ECTexture{
	Texture *texture;
};


void ECTexture_Setup(void *_this);
void ECTexture_Destroy(void *_this);



#endif
