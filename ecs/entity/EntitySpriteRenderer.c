#include "ecs/zg_ecs.h"

void EntitySpriteRenderer_SetDimensions(Entity *_this,uint16_t width, uint16_t height){
	ECSpriteRenderer_SetDimensions(Entity_GetComponent(_this,ENTITY_COMPONENT_SPRITE_RENDERER),width, height);
}


void EntitySpriteRenderer_SetTexture(Entity *_this, Texture *texture){
	EntitySpriteRenderer_SetTexture(Entity_GetComponent(_this,ENTITY_COMPONENT_SPRITE_RENDERER),texture);
}


void EntitySpriteRenderer_SetAlpha(Entity *_this, float _alpha){

	ECSpriteRenderer_SetAlpha(Entity_GetComponent(_this,ENTITY_COMPONENT_SPRITE_RENDERER),_alpha);
}
