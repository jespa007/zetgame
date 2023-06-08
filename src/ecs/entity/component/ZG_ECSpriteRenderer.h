#ifndef _ZG_ECSPRITE_RENDER_H__
#define _ZG_ECSPRITE_RENDER_H__

typedef struct ZG_ECSpriteRenderer ZG_ECSpriteRenderer;

struct ZG_ECSpriteRenderer{
	ZG_EComponentHeader header;
	void 			*data;
};

ZG_EComponentList ZG_ECSpriteRenderer_RequiredComponents(void);
void ZG_ECSpriteRenderer_Setup(void *_this,ZG_ComponentId _id);

//void ZG_ECSpriteRenderer_SetQuadPixels(uint16 width, uint16 height);
void ZG_ECSpriteRenderer_SetDimensions(ZG_ECSpriteRenderer *_this,uint16_t width, uint16_t height);
void ZG_ECSpriteRenderer_SetTexture(ZG_ECSpriteRenderer *_this,ZG_Texture *texture);
void ZG_ECSpriteRenderer_SetAlpha(ZG_ECSpriteRenderer *_this, float _alpha);

void ZG_ECSpriteRenderer_Update(void *_this);
void ZG_ECSpriteRenderer_Destroy(void *_this);






#endif
