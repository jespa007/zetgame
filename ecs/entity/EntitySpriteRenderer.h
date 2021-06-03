#ifndef __ENTITY_SPRITE_RENDER_H__
#define __ENTITY_SPRITE_RENDER_H__

void EntitySpriteRenderer_SetDimensions(Entity *_this,uint16_t width, uint16_t height);
void EntitySpriteRenderer_SetTexture(Entity *_this, Texture *texture);
void EntitySpriteRenderer_SetAlpha(Entity *_this, float _alpha);

#endif
