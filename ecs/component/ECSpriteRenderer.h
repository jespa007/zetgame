#ifndef _EC_SPRITE_RENDER_H__
#define _EC_SPRITE_RENDER_H__

typedef struct ECSpriteRenderer ECSpriteRenderer;

struct ECSpriteRenderer{
	Entity *entity;
	void *data;
};


void ECSpriteRenderer_Setup(void *_this);
void ECSpriteRenderer_Ini(void *_this,Entity *_entity);

//void ECSpriteRenderer_SetQuadPixels(uint16 width, uint16 height);
void ECSpriteRenderer_SetDimensions(ECSpriteRenderer *_this,uint16_t width, uint16_t height);
void ECSpriteRenderer_SetTexture(ECSpriteRenderer *_this,Texture *texture);
void ECSpriteRenderer_SetAlpha(ECSpriteRenderer *_this, float _alpha);

void ECSpriteRenderer_Update(void *_this);
void ECSpriteRenderer_Destroy(void *_this);






#endif
