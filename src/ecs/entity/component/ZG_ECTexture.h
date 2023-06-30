#ifndef __ZG_ECTEXTURE_H__
#define __ZG_ECTEXTURE_H__

typedef struct ZG_ECTexture ZG_ECTexture;

struct ZG_ECTexture{
	ZG_EntityHeader  header;
	ZG_Texture 		*texture;
};


//void ZG_ECTexture_Setup(void *_this,ZG_ComponentId _id);
//void ZG_ECTexture_SetTexture(ZG_ECTexture *_this,ZG_Texture *_texture);


#endif
