#ifndef _ZG_ECSPRITE_RENDER_H__
#define _ZG_ECSPRITE_RENDER_H__

typedef struct ZG_ECSpriteRenderer ZG_ECSpriteRenderer;

struct ZG_ECSpriteRenderer{
	ZG_EntityHeader header;
	void 			*data;
};

//---
ZG_ComponentList ZG_ECSpriteRenderer_RequiredComponents(void);
void ZG_ECSpriteRenderer_OnCreate(void *_this, ZG_Entity _entity);
void ZG_ECSpriteRenderer_Update(void *_this, ZG_Entity _entity);
void ZG_ECSpriteRenderer_OnDestroy(void *_this, ZG_Entity _entity);
//---


//void ZG_ECSpriteRenderer_SetQuadPixels(uint16 width, uint16 height);
void ZG_ECSpriteRenderer_SetDimensions(ZG_ECSpriteRenderer *_this,uint16_t width, uint16_t height);
void ZG_ECSpriteRenderer_SetTexture(ZG_ECSpriteRenderer *_this,ZG_Texture *texture);
void ZG_ECSpriteRenderer_SetAlpha(ZG_ECSpriteRenderer *_this, float _alpha);







#endif
